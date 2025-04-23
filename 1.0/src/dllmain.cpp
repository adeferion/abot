#include <windows.h>
#include <shellapi.h>
#include <fstream>
#include <format>
#include <cmath>
#include <cocos2d.h>
#include <gd.h>
#include <string.h>
#include "console.h"
#include "fpsBypass.h"
#include "playLayer.h"
#include "spamBot.h"
#include <imgui_hook.h>
#include <imgui_internal.h>
#include <imgui.h>
#include "hwid.h"

#define VK_RCONTROL 0xA3 // or 163 in decimal

    using namespace cocos2d;

bool noclip = false;
bool practice_music_hack = true;
bool practice_coins = false;
bool anticheat_bypass = true;
bool ignore_esc = false;
bool no_respawn_flash = true;
bool disable_death_effects = true;

bool show = false;
bool inited = false;
bool sortWindows = true;
bool sortWindows2 = true;

ImVec2 buttonSize = {230.f - 18.f, 24.f};

bool isRecording;

const char* converterTypes[]{ "Plain Text (.txt)" };
int converterType = 0;

int replay_select_player_p1 = 1;
int replay_current = 0;

char replay_name[128] = "";
vector<string> replay_list;
bool openned = false;

bool overwrite = false;
bool loading = false;

bool isLoggedIn = false;  // To track if the user is logged in
bool showLoginWindow = true;
char username[128] = "";   // Username input
char password[128] = "";   // Password input
char loginError[256] = ""; // Error message if login fails

static float fps_input = FPSMultiplier::g_target_fps;

void RenderLogin() {
    ImVec2 windowSize = ImVec2(250, 120);
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 centerPos = ImVec2(
        viewport->Pos.x + (viewport->Size.x - windowSize.x) * 0.5f,
        viewport->Pos.y + (viewport->Size.y - windowSize.y) * 0.1f
    );

    // Set size, position, and focus
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always);
    ImGui::SetNextWindowFocus();

    // Begin modal window
    ImGui::Begin("aBot Login", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar
    );

    ImGui::InputText("Username", username, IM_ARRAYSIZE(username));
    ImGui::InputText("Password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
    
    if (ImGui::Button("Login", buttonSize)) {
        std::string hwid = GetHWID();
        if (!CheckCredentialsOnline(username, password, hwid)) {
            strcpy_s(loginError, "Invalid Credentials");
        } else {
            isLoggedIn = true;
            showLoginWindow = false;
        }
    }

    if (strlen(loginError) > 0) {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), loginError);
    }

    ImGui::End();
}

void ConfirmMessage(float x, float y) {
    if (!overwrite) return;

    if (loading) {
        playLayer::clearMacro();
        playLayer::loadReplay(".aBot/" + (std::string)replay_name);
    }
    else {
        playLayer::clearMacro();
    }

    overwrite = false;
}

void RenderMain() {
    if (!isLoggedIn) {
        if (GetAsyncKeyState(VK_RCONTROL) & 0x8000)
            showLoginWindow = true;

        if (showLoginWindow) {
            RenderLogin();
            return;
        }
    }

    // Now we're logged in, proceed with rendering the main window
    CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents(!ImGui::GetIO().WantCaptureMouse);

    if (show) {
        ImGui::Begin("General", &show, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        if (!inited) {
            ImGui::SetWindowSize(ImVec2(230,350));
            if (sortWindows || sortWindows2)
                ImGui::SetWindowPos(ImVec2(10, 10));
            inited = true;
        }
        
        // Get the window width and calculate the offset to center elements
        float window_width = ImGui::GetWindowWidth();
        float content_width = ImGui::GetContentRegionAvail().x;
        float offset = (window_width - content_width) * 0.5f; // This will center the content
        
        // Start from the center and then adjust the positioning for each element
        ImGui::SetCursorPosX(offset);
        
        if (ImGui::BeginChild("##RigthSide", ImVec2(content_width, ImGui::GetContentRegionAvail().y), true))
        {
            int currentMode = playLayer::mode;
            const char* modes[] = { "Disabled", "Record", "Playback" };
            if (ImGui::Combo("Mode", &currentMode, modes, IM_ARRAYSIZE(modes))) {
                if (currentMode == 0) { // Disabled
                    playLayer::checkpoints_p1.clear();
                    playLayer::checkpoints_p2.clear();
                }
                else if (currentMode == 1) { // Record
                    if (practice_music_hack && anticheat_bypass) {
                        playLayer::replay_p1.clear();
                        playLayer::replay_p2.clear();
                        playLayer::checkpoints_p1.clear();
                        playLayer::checkpoints_p2.clear();
                    } else {
                        currentMode = 0;
                    }
                }
                // No special logic needed for Playback
                playLayer::mode = currentMode;
            }
        
            if (currentMode == 1 && (!practice_music_hack || !anticheat_bypass)) {
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("To record, please enable \"Practice Music Hack\" and \"Anticheat bypass\"");
                }
            }
        
            ImGui::Checkbox("Bruteforce", &playLayer::accuracy_fix);
            if (playLayer::accuracy_fix) { 
                ImGui::Checkbox("High FPS Rotation Fix", &playLayer::rotation_fix); 
            }
            ImGui::Checkbox("Ignore Inputs on Playback", &playLayer::ignore_input);
        
            ImGui::Separator();    
        
            ImGui::Text("Frame:");
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255)); // Yellow
            ImGui::Text("%i", playLayer::frame);
            ImGui::PopStyleColor();             
        
            ImGui::Separator();
        
            ImGui::PushItemWidth(buttonSize.x);
            ImGui::InputText("##replayinput", replay_name, IM_ARRAYSIZE(replay_name));
            auto itemx = ImGui::GetItemRectMin().x;
            auto itemy = ImGui::GetItemRectMax().y;
            auto itemw = ImGui::GetItemRectSize().x;
            ImGui::SameLine(0);
            if (ImGui::ArrowButton("##comboopen", openned ? ImGuiDir_Up : ImGuiDir_Down)) {
                openned = !openned;
                if (openned) {
                    replay_list.clear();
                    for (const auto& entry : filesystem::directory_iterator(".aBot")) {
                        replay_list.push_back(entry.path().filename().string());
                    }
                }
            }
            if (openned) {
                ImGui::SetNextWindowPos(ImVec2(itemx, itemy + 4));
                ImGui::SetNextWindowSize(ImVec2(itemw + ImGui::GetItemRectSize().x, NULL));
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1);
                ImGui::Begin("##MacroList", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
                for (int i = 0; i < (int)replay_list.size(); i++) {
                    if (ImGui::MenuItem(replay_list[i].c_str())) {
                        strcpy_s(replay_name, replay_list[i].c_str());
                        openned = false;
                    }
                }
                ImGui::End();
                ImGui::PopStyleVar();
            }
        
            if (ImGui::Button("Save", {buttonSize.x / 2, buttonSize.y})) {
                std::string filename = (std::string)replay_name;
                if (!filename.ends_with(".replay"))
                    filename += ".replay";
        
                playLayer::saveReplay(".aBot/" + filename);
            }
        
            ImGui::SameLine();
        
            if (ImGui::Button("Load", {buttonSize.x / 2 - 5, buttonSize.y})) {
                std::string filename = (std::string)replay_name;
                if (!filename.ends_with(".replay"))
                    filename += ".replay";
                
                    if (playLayer::replay_p1.empty()) {
                        playLayer::clearMacro();
                        playLayer::loadReplay(".aBot/" + filename);
                    }
                    else {
                        overwrite = true;
                        loading = true;
                    }
                }

                if (ImGui::Button("Clear", buttonSize)) {
                    if (playLayer::replay_p1.empty()) playLayer::clearMacro();
                    else  {
                        overwrite = true;
                        loading = false;
                    }
                }
                
                if (overwrite) {
                    ConfirmMessage(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y + 4);
                }     

                    ImGui::Begin("Assist", &show, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

                    ImGui::SetWindowSize(ImVec2(230,185));
                    if (sortWindows || sortWindows2)
                        ImGui::SetWindowPos(ImVec2(250, 10));
                
                        if (ImGui::InputFloat("##FPS", &fps_input, 1.f, 100.f, "%.2f")) {
                            
                        }

                        if (ImGui::Button("Set FPS", buttonSize)) {
                            FPSMultiplier::g_target_fps = fps_input;
                            FPSMultiplier::g_enabled = true;
                        }
                        
                        ImGui::Separator();
                        
                        if (ImGui::InputFloat("##Speed", &playLayer::speedvalue, 0.01f, 0.f, "%.2f")) {
                        
                        }
                        
                        if (ImGui::Button("Set speed", buttonSize)) {
                            if (playLayer::speedvalue != 0) {
                                CCDirector::sharedDirector()->getScheduler()->setTimeScale(playLayer::speedvalue); 
                            }
                        }                        
                
                    ImGui::End();               

                    ImGui::Begin("Editor", &show, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

                    ImGui::SetWindowSize(ImVec2(400,350));
                    if (sortWindows)
                        ImGui::SetWindowPos(ImVec2(490, 10));

                    if (ImGui::BeginChild("##LeftSideEditor", ImVec2(120, ImGui::GetContentRegionAvail().y), true)) {
                        for (int i = 0; i < (int)playLayer::replay_p1.size(); i++) {
                            const bool is_selected = (replay_current == i);
                            ImGui::PushItemWidth(120.f);
                            if (ImGui::Selectable(to_string(playLayer::replay_p1[i].frame).c_str(), is_selected))
                                replay_current = i;
                    
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndChild();
                    }
                    
                    ImGui::SameLine();
                    
                    if (ImGui::BeginChild("##RightSideEditor", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true)) {
                        if (!playLayer::replay_p1.empty() && replay_select_player_p1) {
                            ImGui::DragFloat("##POSXP1", &playLayer::replay_p1[replay_current].pos_x, 0.000001f, -1, FLT_MAX, "Position X: %f");
                            ImGui::DragFloat("##POSYP1", &playLayer::replay_p1[replay_current].pos_y, 0.000001f, -1, FLT_MAX, "Position Y: %f");
                            ImGui::DragFloat("##ROTATEP1", &playLayer::replay_p1[replay_current].rotation, 0.000001f, -1, FLT_MAX, "Rotation: %f");
                            ImGui::DragFloat("##YVELP1", &playLayer::replay_p1[replay_current].y_vel, 0.000001f, -1, FLT_MAX, "Y Vel: %f");
                            ImGui::DragInt("##DOWNP1", &playLayer::replay_p1[replay_current].down, 1, -1, 1, "Down: %i");
                        }
                    
                        if (!playLayer::replay_p2.empty() && !replay_select_player_p1) {
                            ImGui::DragFloat("##POSXP2", &playLayer::replay_p2[replay_current].pos_x, 0.000001f, -1, FLT_MAX, "Position X: %f");
                            ImGui::DragFloat("##POSYP2", &playLayer::replay_p2[replay_current].pos_y, 0.000001f, -1, FLT_MAX, "Position Y: %f");
                            ImGui::DragFloat("##ROTATEP2", &playLayer::replay_p2[replay_current].rotation, 0.000001f, -1, FLT_MAX, "Rotation: %f");
                            ImGui::DragFloat("##YVELP2", &playLayer::replay_p2[replay_current].y_vel, 0.000001f, -1, FLT_MAX, "Y Vel: %f");
                            ImGui::DragInt("##DOWNP2", &playLayer::replay_p2[replay_current].down, 1, -1, 1, "Down: %i");
                        }
                    
                        ImGui::Text("Note: -1 value does nothing with\nplayer");
                        ImGui::Separator();
                        ImGui::RadioButton("Player 1", &replay_select_player_p1, 1);
                        ImGui::SameLine();
                        ImGui::RadioButton("Player 2", &replay_select_player_p1, 0);
                        ImGui::Separator();
                        ImGui::EndChild();
                    }

                    ImGui::Begin("Converter", &show, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

                    ImGui::SetWindowSize(ImVec2(275,133));
                    if (sortWindows)
                        ImGui::SetWindowPos(ImVec2(900, 10));

                    ImGui::Combo("##ConverterType", &converterType, converterTypes, IM_ARRAYSIZE(converterTypes));
                    
                    if (ImGui::Button("Convert")) {
                        if (converterType == 0) {
                            std::ofstream out(".aBot/converted.txt");
                            out << FPSMultiplier::g_target_fps << "\n";
                            for (int i = 0; i < (int)playLayer::replay_p1.size(); i++) {
                                if (i == 0 || (playLayer::replay_p1[i].down == playLayer::replay_p1[i - 1].down && playLayer::replay_p2[i].down == playLayer::replay_p2[i - 1].down))
                                    continue;
                                out << i << " " << playLayer::replay_p1[i].down << " " << playLayer::replay_p2[i].down << "\n";
                            }
                            out.close();
                        }
                    }
                    
                    ImGui::SameLine();
                    
                    if (ImGui::Button("ILL Replay Converter")) {
                        ShellExecuteA(0, "open", "https://zodiac-ill.github.io/gd-macro-converter/", 0, 0, SW_SHOWNORMAL);
                    }
                    
                    if (converterType == 0) {
                        ImGui::Text("Replay will be saved to 'aBot/converted.txt'");
                    }

                    ImGui::Begin("Hacks", &show, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

                    ImGui::SetWindowSize(ImVec2(275,200));
                    if (sortWindows) {
                        ImGui::SetWindowPos(ImVec2(900, 160));
                        sortWindows = false;
                    }

                    if (ImGui::Checkbox("NoClip", &noclip)) {
                        if (noclip) {
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x20A23C), "\xE9\x79\x06\x00\x00", 5, NULL);
                        } else {
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x20A23C), "\x6A\x14\x8B\xCB\xFF", 5, NULL);
                        }
                    }
                    
                    if (ImGui::Checkbox("Practice Music Hack", &practice_music_hack)) {
                        if (practice_music_hack) {
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x20C925), "\x90\x90\x90\x90\x90\x90", 6, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x20D143), "\x90\x90", 2, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x20A563), "\x90\x90", 2, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x20A595), "\x90\x90", 2, NULL);
                        } else {
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x20C925), "\x0F\x85\xF7\x00\x00\x00", 6, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x20D143), "\x75\x41", 2, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x20A563), "\x75\x3E", 2, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x20A595), "\x75\x0C", 2, NULL);
                        }
                    }
                    
                    if (ImGui::Checkbox("Ignore ESC", &ignore_esc)) {
                        if (ignore_esc) {
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1E644C), "\x90\x90\x90\x90\x90", 5, NULL);
                        } else {
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1E644C), "\xE8\xBF\x73\x02\x00", 5, NULL);
                        }
                    }
                    
                    if (ImGui::Checkbox("No Respawn Flash", &no_respawn_flash)) {
                        if (no_respawn_flash) {
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1EF36D), "\xE9\xA8\x00\x00\x00\x90", 6, NULL);
                        } else {
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1EF36D), "\x0F\x85\xA7\x00\x00\x00", 6, NULL);
                        }
                    }
                    
                    if (ImGui::Checkbox("Disable Death Effects", &disable_death_effects)) {
                        if (disable_death_effects) {
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1EFBA4), "\x90\x90\x90\x90\x90", 5, NULL);
                        } else {
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1EFBA4), "\xE8\x37\x00\x00\x00", 5, NULL);
                        }
                    }
                    
                    if (ImGui::Checkbox("Practice Coins", &practice_coins)) {
                        if (practice_coins) {
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x204F10), "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 13, NULL);
                        } else {
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x204F10), "\x80\xBE\x95\x04\x00\x00\x00\x0F\x85\xDE\x00\x00\x00", 13, NULL);
                        }
                    }
                    
                    if (ImGui::Checkbox("Anticheat Bypass", &anticheat_bypass)) {
                        if (anticheat_bypass) {
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x202AAA), "\xEB\x2E", 2, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x15FC2E), "\xEB", 1, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x20D3B3), "\x90\x90\x90\x90\x90", 5, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1FF7A2), "\x90\x90", 2, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x18B2B4), "\xB0\x01", 2, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x20C4E6), "\xE9\xD7\x00\x00\x00\x90", 6, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1FD557), "\xEB\x0C", 2, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1FD742), "\xC7\x87\xE0\x02\x00\x00\x01\x00\x00\x00\xC7\x87\xE4\x02\x00\x00\x00\x00\x00\x00\x90\x90\x90\x90\x90\x90", 26, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1FD756), "\x90\x90\x90\x90\x90\x90", 6, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1FD79A), "\x90\x90\x90\x90\x90\x90", 6, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1FD7AF), "\x90\x90\x90\x90\x90\x90", 6, NULL);
                        } else {
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x202AAA), "\x74\x2E", 2, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x15FC2E), "\x74", 1, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x20D3B3), "\xE8\x58\x04\x00\x00", 5, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1FF7A2), "\x74\x6E", 2, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x18B2B4), "\x88\xD8", 2, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x20C4E6), "\x0F\x85\xD6\x00\x00\x00", 6, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1FD557), "\x74\x0C", 2, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1FD742), "\x80\xBF\xDD\x02\x00\x00\x00\x0F\x85\x0A\xFE\xFF\xFF\x80\xBF\x34\x05\x00\x00\x00\x0F\x84\xFD\xFD\xFF\xFF", 26, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1FD756), "\x0F\x84\xFD\xFD\xFF\xFF", 6, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1FD79A), "\x0F\x84\xB9\xFD\xFF\xFF", 6, NULL);
                            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1FD7AF), "\x0F\x85\xA4\xFD\xFF\xFF", 6, NULL);
                        }
                    }

            ImGui::Begin("Settings", &show, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse); {

                ImGui::SetWindowSize(ImVec2(230,155));
                if (sortWindows || sortWindows2)
                {
                    ImGui::SetWindowPos(ImVec2(250, 205));
                    sortWindows2 = false;
                }
               
                ImGui::Checkbox("Checkpoint Fix", &playLayer::practice_fix);
                ImGui::Separator();
                ImGui::Checkbox("FPS Bypass", &FPSMultiplier::fpsbypass_enabled);
                ImGui::Separator();
                ImGui::Checkbox("Lock Delta on Playback", &FPSMultiplier::g_enabled);
            }
        }

        ImGui::End();
    }
}

inline void(__thiscall* dispatchKeyboardMSG)(void* self, int key, bool down);
void __fastcall dispatchKeyboardMSGHook(void* self, void*, int key, bool down) {
    dispatchKeyboardMSG(self, key, down);
    auto pl = gd::GameManager::sharedState()->getPlayLayer();
    if (down && (GetAsyncKeyState(VK_RCONTROL) & 0x8000)) {
        show = !show;
    }

    if (pl && down && key == 'C') {
        FPSMultiplier::frame_advance = true;
        FPSMultiplier::nextframe = true;
    }

    if (pl && down && key == 'F') {
        FPSMultiplier::frame_advance = false;
        FPSMultiplier::nextframe = false;
    }

    if (down && key == 'P') {
        if (playLayer::mode != 2) playLayer::mode = 2;
        else playLayer::mode = 0;
    }

}

DWORD WINAPI Main(void* hModule) {
    srand((uint32_t)time(NULL));
    if (!std::filesystem::is_directory(".aBot") || !std::filesystem::exists(".aBot"))
        std::filesystem::create_directory(".aBot");

    ImGuiHook::Load(RenderMain);
    MH_Initialize();
    FPSMultiplier::Setup();
    playLayer::mem_init();
    MH_CreateHook(
        (PVOID)(GetProcAddress(GetModuleHandleA("libcocos2d.dll"), "?dispatchKeyboardMSG@CCKeyboardDispatcher@cocos2d@@QAE_NW4enumKeyCodes@2@_N@Z")),
        dispatchKeyboardMSGHook,
        (LPVOID*)&dispatchKeyboardMSG
    );
    MH_EnableHook(MH_ALL_HOOKS);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        CreateThread(0, 0x1000, Main, hModule, 0, 0);
        break;
    }
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
