#include "SettingsUI.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_internal.h"
#include "SoundManager.h"

void SettingsUI::Render()
{
    constexpr float SETTING_WIDTH = 320.0f;
    constexpr float SETTING_HEIGHT = 260.0f;

    constexpr float LABEL_WIDTH = 40.0f;
    constexpr float SLIDER_WIDTH = 210.0f;
    constexpr float ROW_WIDTH = LABEL_WIDTH + SLIDER_WIDTH;

    constexpr float CLOSE_BUTTON_WIDTH = 120.0f;
    constexpr float CLOSE_BUTTON_HEIGHT = 35.0f;

    // =========================
    // Settings Popup
    // =========================

    ImGui::SetNextWindowSize(
        ImVec2(SETTING_WIDTH, SETTING_HEIGHT),
        ImGuiCond_Always
    );

    if (ImGui::BeginPopupModal(
        "Settings",
        nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse))
    {
        float bgmVolume = USoundManager::GetInstance()->GetBGMVolume();

        float sfxVolume = USoundManager::GetInstance()->GetSFXVolume();

        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        const char* settingsTitle = "SETTINGS";
        float settingsTitleWidth = ImGui::CalcTextSize(settingsTitle).x;

        ImGui::SetCursorPosX(
            (SETTING_WIDTH - settingsTitleWidth) * 0.5f
        );

        ImGui::Text("%s", settingsTitle);

        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 25.0f));

        float startX = (SETTING_WIDTH - ROW_WIDTH) * 0.5f;

        // BGM
        ImGui::SetCursorPosX(startX);
        ImGui::Text("BGM");

        ImGui::SameLine(startX + LABEL_WIDTH);
        ImGui::SetNextItemWidth(SLIDER_WIDTH);

        
        const float bgmstart = 0.0f;
        const float bgmend = 0.5f;

        float bgmpercent = (bgmVolume - bgmstart) / (bgmend - bgmstart) * 100.0f;

        if (ImGui::SliderFloat("##BGM",&bgmpercent,0.0f,100.0f,"%.0f")) {
            bgmVolume = bgmstart + (bgmpercent / 100.0f) * (bgmend - bgmstart);

            USoundManager::GetInstance()->SetBGMVolume(bgmVolume);
        }

        ImGui::Dummy(ImVec2(0.0f, 20.0f));

        // SFX
        ImGui::SetCursorPosX(startX);
        ImGui::Text("SFX");

        ImGui::SameLine(startX + LABEL_WIDTH);
        ImGui::SetNextItemWidth(SLIDER_WIDTH);

        const float sfxstart = 0.0f;
        const float sfxend = 0.7f;

        float sfxpercent = (sfxVolume - sfxstart) / (sfxend - sfxstart) * 100.0f;

        if (ImGui::SliderFloat("##SFX",&sfxpercent,0.0f,100.0f,"%.0f")) {
            sfxVolume = sfxstart + (sfxpercent / 100.0f) * (sfxend - sfxstart);
            USoundManager::GetInstance()->SetSFXVolume(sfxVolume);
        }

        ImGui::Dummy(ImVec2(0.0f, 25.0f));

        ImGui::SetCursorPosX(
            (SETTING_WIDTH - CLOSE_BUTTON_WIDTH) * 0.5f
        );

        if (ImGui::Button("CLOSE",ImVec2(CLOSE_BUTTON_WIDTH, CLOSE_BUTTON_HEIGHT)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}