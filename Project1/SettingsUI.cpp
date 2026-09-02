#include "SettingsUI.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_internal.h"
#include "SoundManager.h"

void SettingsUI::Render()
{
    constexpr float SETTING_WIDTH = 360.0f;
    constexpr float SETTING_HEIGHT = 260.0f;

    constexpr float LABEL_WIDTH = 50.0f;
    constexpr float SLIDER_WIDTH = 220.0f;
    constexpr float ROW_WIDTH = LABEL_WIDTH + SLIDER_WIDTH;

    constexpr float CLOSE_BUTTON_WIDTH = 220.0f;
    constexpr float CLOSE_BUTTON_HEIGHT = 40.0f;

    // =========================
    // Settings Popup
    // =========================

    ImGuiIO& io = ImGui::GetIO();

    // 화면 중앙에 배치
    ImGui::SetNextWindowPos(
        ImVec2(
            io.DisplaySize.x * 0.5f,
            io.DisplaySize.y * 0.5f
        ),
        ImGuiCond_Appearing,
        ImVec2(0.5f, 0.5f)
    );

    ImGui::SetNextWindowSize(
        ImVec2(SETTING_WIDTH, SETTING_HEIGHT),
        ImGuiCond_Always
    );

    // Settings 모달 배경
    ImGui::PushStyleVar(
        ImGuiStyleVar_WindowPadding,
        ImVec2(25.0f, 22.0f)
    );

    ImGui::PushStyleVar(
        ImGuiStyleVar_WindowRounding,
        12.0f
    );

    ImGui::PushStyleVar(
        ImGuiStyleVar_WindowBorderSize,
        1.0f
    );

    ImGui::PushStyleVar(
        ImGuiStyleVar_FrameRounding,
        7.0f
    );

    ImGui::PushStyleColor(
        ImGuiCol_PopupBg,
        ImVec4(0.04f, 0.05f, 0.07f, 0.99f)
    );

    ImGui::PushStyleColor(
        ImGuiCol_Border,
        ImVec4(0.15f, 0.50f, 0.60f, 0.8f)
    );

    if (ImGui::BeginPopupModal(
        "Settings",
        nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse))
    {
        float bgmVolume = USoundManager::GetInstance()->GetBGMVolume();

        float sfxVolume = USoundManager::GetInstance()->GetSFXVolume();

        // Settings 제목
        ImGui::SetWindowFontScale(1.6f);

        const char* settingsTitle = "SETTINGS";
        float settingsTitleWidth = ImGui::CalcTextSize(settingsTitle).x;

        ImGui::SetCursorPosX(
            (SETTING_WIDTH - settingsTitleWidth) * 0.5f
        );

        ImGui::Text("%s", settingsTitle);

        ImGui::SetWindowFontScale(1.0f);

        const char* subtitle = "Audio Settings";
        float subtitleWidth =
            ImGui::CalcTextSize(subtitle).x;

        ImGui::SetCursorPosX(
            (SETTING_WIDTH - subtitleWidth) * 0.5f
        );

        ImGui::TextDisabled("%s", subtitle);

        ImGui::Dummy(ImVec2(0.0f, 12.0f));

        float startX = (SETTING_WIDTH - ROW_WIDTH) * 0.5f;

        // 슬라이더 스타일
        ImGui::PushStyleColor(
            ImGuiCol_FrameBg,
            ImVec4(0.08f, 0.13f, 0.17f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_FrameBgHovered,
            ImVec4(0.10f, 0.20f, 0.25f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_SliderGrab,
            ImVec4(0.15f, 0.60f, 0.70f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_SliderGrabActive,
            ImVec4(0.20f, 0.75f, 0.85f, 1.0f)
        );

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

        ImGui::Dummy(ImVec2(0.0f, 14.0f));

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

        ImGui::PopStyleColor(4);

        ImGui::Dummy(ImVec2(0.0f, 20.0f));

        // CLOSE 버튼
        ImGui::SetCursorPosX(
            (SETTING_WIDTH - CLOSE_BUTTON_WIDTH) * 0.5f
        );

        ImGui::PushStyleColor(
            ImGuiCol_Button,
            ImVec4(0.10f, 0.45f, 0.55f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_ButtonHovered,
            ImVec4(0.15f, 0.60f, 0.70f, 1.0f)
        );

        ImGui::PushStyleColor(
            ImGuiCol_ButtonActive,
            ImVec4(0.20f, 0.75f, 0.85f, 1.0f)
        );

        if (ImGui::Button(
            "CLOSE",
            ImVec2(
                CLOSE_BUTTON_WIDTH,
                CLOSE_BUTTON_HEIGHT)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::PopStyleColor(3);

        // ESC 키 입력 시 Settings 닫기
        if (ImGui::IsKeyPressed(ImGuiKey_Escape))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar(4);
}