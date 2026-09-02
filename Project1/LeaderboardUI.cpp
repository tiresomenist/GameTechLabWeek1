#include "LeaderboardUI.h"
#include "LeaderboardManager.h"
#include "ImGui/imgui.h"

void LeaderboardUI::Render()
{
	constexpr float LEADERBOARD_WIDTH = 430.0f;

	constexpr float CLOSE_BUTTON_WIDTH = 220.0f;
	constexpr float CLOSE_BUTTON_HEIGHT = 40.0f;

	constexpr float HEADER_HEIGHT = 32.0f;
	constexpr float ROW_HEIGHT = 34.0f;
	constexpr int MAX_VISIBLE_ROWS = 5;

	const auto& scores =
		LeaderboardManager::GetInstance()->GetScores();

	// 점수 개수에 따라 목록 높이 조절
	int visibleRows =
		static_cast<int>(scores.size());

	if (visibleRows < 1)
		visibleRows = 1;

	if (visibleRows > MAX_VISIBLE_ROWS)
		visibleRows = MAX_VISIBLE_ROWS;

	float listHeight =
		HEADER_HEIGHT +
		ROW_HEIGHT * visibleRows +
		16.0f;

	float leaderboardHeight =
		listHeight + 165.0f;

	ImGuiIO& io = ImGui::GetIO();

	// =========================
	// Leaderboard Popup
	// =========================

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
		ImVec2(
			LEADERBOARD_WIDTH,
			leaderboardHeight
		),
		ImGuiCond_Always
	);

	// Leaderboard 모달 배경
	ImGui::PushStyleVar(
		ImGuiStyleVar_WindowPadding,
		ImVec2(25.0f, 22.0f)
	);

	ImGui::PushStyleVar(
		ImGuiStyleVar_WindowRounding,
		14.0f
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
		"Leaderboard",
		nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse))
	{
		// 최상단 타이틀
		ImGui::SetWindowFontScale(1.8f);

		const char* leaderboardTitle =
			"LEADERBOARD";

		float leaderboardTitleWidth =
			ImGui::CalcTextSize(
				leaderboardTitle
			).x;

		ImGui::SetCursorPosX(
			(ImGui::GetWindowWidth() -
				leaderboardTitleWidth) * 0.5f
		);

		ImGui::Text(
			"%s",
			leaderboardTitle
		);

		ImGui::SetWindowFontScale(1.0f);

		const char* subtitle =
			"TOP SCORES";

		float subtitleWidth =
			ImGui::CalcTextSize(
				subtitle
			).x;

		ImGui::SetCursorPosX(
			(ImGui::GetWindowWidth() -
				subtitleWidth) * 0.5f
		);

		ImGui::TextDisabled(
			"%s",
			subtitle
		);

		ImGui::Dummy(
			ImVec2(0.0f, 12.0f)
		);

		// 순위 목록
		ImGui::PushStyleVar(
			ImGuiStyleVar_ChildRounding,
			8.0f
		);

		ImGui::PushStyleVar(
			ImGuiStyleVar_ChildBorderSize,
			1.0f
		);

		ImGui::PushStyleVar(
			ImGuiStyleVar_WindowPadding,
			ImVec2(14.0f, 8.0f)
		);

		ImGui::PushStyleColor(
			ImGuiCol_ChildBg,
			ImVec4(
				0.07f,
				0.085f,
				0.11f,
				0.95f
			)
		);

		ImGui::PushStyleColor(
			ImGuiCol_Border,
			ImVec4(
				0.15f,
				0.50f,
				0.60f,
				0.45f
			)
		);

		ImGui::BeginChild(
			"LeaderboardList",
			ImVec2(
				0.0f,
				listHeight
			),
			true
		);

		if (scores.empty())
		{
			// 저장된 점수가 없을 때
			const char* emptyText =
				"NO SCORES YET";

			float emptyWidth =
				ImGui::CalcTextSize(
					emptyText
				).x;

			ImGui::SetCursorPosY(
				(listHeight -
					ImGui::GetTextLineHeight()) *
				0.5f - 8.0f
			);

			ImGui::SetCursorPosX(
				(ImGui::GetWindowWidth() -
					emptyWidth) * 0.5f
			);

			ImGui::TextDisabled(
				"%s",
				emptyText
			);
		}
		else
		{
			if (ImGui::BeginTable(
				"LeaderboardTable",
				3,
				ImGuiTableFlags_SizingStretchProp |
				ImGuiTableFlags_RowBg))
			{
				ImGui::TableSetupColumn(
					"RANK",
					ImGuiTableColumnFlags_WidthFixed,
					70.0f
				);

				ImGui::TableSetupColumn(
					"PLAYER",
					ImGuiTableColumnFlags_WidthStretch
				);

				ImGui::TableSetupColumn(
					"SCORE",
					ImGuiTableColumnFlags_WidthFixed,
					90.0f
				);

				// 컬럼 제목
				ImGui::TableNextRow(
					ImGuiTableRowFlags_None,
					HEADER_HEIGHT
				);

				ImGui::TableSetColumnIndex(0);

				ImGui::SetCursorPosY(
					ImGui::GetCursorPosY() +
					6.0f
				);

				ImGui::TextDisabled("RANK");

				ImGui::TableSetColumnIndex(1);

				ImGui::SetCursorPosY(
					ImGui::GetCursorPosY() +
					6.0f
				);

				ImGui::TextDisabled("PLAYER");

				ImGui::TableSetColumnIndex(2);

				ImGui::SetCursorPosY(
					ImGui::GetCursorPosY() +
					6.0f
				);

				ImGui::TextDisabled("SCORE");

				int rank = 0;

				// 팀원 이름
				for (auto& entry : scores)
				{
					rank++;

					ImGui::TableNextRow(
						ImGuiTableRowFlags_None,
						ROW_HEIGHT
					);

					// 순위
					ImGui::TableSetColumnIndex(0);

					ImGui::SetCursorPosY(
						ImGui::GetCursorPosY() +
						7.0f
					);

					if (rank == 1)
					{
						ImGui::TextColored(
							ImVec4(
								0.95f,
								0.80f,
								0.25f,
								1.0f
							),
							"#1"
						);
					}
					else
					{
						ImGui::TextDisabled(
							"#%d",
							rank
						);
					}

					// 플레이어 이름
					ImGui::TableSetColumnIndex(1);

					ImGui::SetCursorPosY(
						ImGui::GetCursorPosY() +
						7.0f
					);

					ImGui::Text(
						"%s",
						entry.first.c_str()
					);

					// 점수
					ImGui::TableSetColumnIndex(2);

					ImGui::SetCursorPosY(
						ImGui::GetCursorPosY() +
						7.0f
					);

					ImGui::Text(
						"%d",
						entry.second
					);
				}

				ImGui::EndTable();
			}
		}

		ImGui::EndChild();

		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(3);

		ImGui::Dummy(
			ImVec2(0.0f, 12.0f)
		);

		// Close
		ImGui::SetCursorPosX(
			(ImGui::GetWindowWidth() -
				CLOSE_BUTTON_WIDTH) * 0.5f
		);

		ImGui::PushStyleColor(
			ImGuiCol_Button,
			ImVec4(
				0.10f,
				0.45f,
				0.55f,
				1.0f
			)
		);

		ImGui::PushStyleColor(
			ImGuiCol_ButtonHovered,
			ImVec4(
				0.15f,
				0.60f,
				0.70f,
				1.0f
			)
		);

		ImGui::PushStyleColor(
			ImGuiCol_ButtonActive,
			ImVec4(
				0.20f,
				0.75f,
				0.85f,
				1.0f
			)
		);

		if (ImGui::Button(
			"CLOSE",
			ImVec2(
				CLOSE_BUTTON_WIDTH,
				CLOSE_BUTTON_HEIGHT
			)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::PopStyleColor(3);

		// ESC 키 입력 시 Leaderboard 닫기
		if (ImGui::IsKeyPressed(
			ImGuiKey_Escape))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar(4);
}