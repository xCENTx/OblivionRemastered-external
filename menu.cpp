#include "menu.h"

Menu::Menu()
{
    //  establish overlay elements
    elements.bIsShown = bShowMenu;
    elements.Menu = std::bind(&Menu::Draw, this);
    elements.Shroud = std::bind(&Menu::SHROUD, this);
    elements.Hud = std::bind(&Menu::HUD, this);

    bRunning = g_memory.bAttached;
}

Menu::~Menu()
{
    elements = DxWindow::SOverlay();
    bRunning = false;
}

void Menu::Draw()
{
    if (!bShowMenu)
        return;

    MainMenu();
}

ImRect Menu::GetImGuiMenuBounds()
{
    const ImVec2& posClone = g_dxWindow->GetCloneWindowPos();   //  get the position of the cloned application window
    const ImVec2& szClone = g_dxWindow->GetCloneWindowSize();   //  get the size of the cloned application window
    const ImVec2& halfClone = szClone * .5;                     //  half application window size

    //  Get Window Size
    ImVec2 szMenu(halfClone);           //  overlay imgui menu window size
    ImVec2 szMenuMax(800.f, 600.f);     //  max overlay imgui menu window size
    szMenu.x = (szMenu.x > szMenuMax.x) ? szMenuMax.x : szMenu.x;
    szMenu.y = (szMenu.y > szMenuMax.y) ? szMenuMax.y : szMenu.y;

	//  Get Window Position
	ImVec2 posMenu = posClone + halfClone - szMenu * .5;   //  overlay imgui menu window position
	return ImRect(posMenu, posMenu + szMenu);
}

ImRect Menu::GetCloneOverlayBounds()
{
    return ImRect(
        g_dxWindow->GetCloneWindowPos(), 
        g_dxWindow->GetCloneWindowPos() + g_dxWindow->GetCloneWindowSize()
    );
}

ImRect Menu::GetClientScreenBounds()
{
	return ImRect(
		ImVec2(0, 0),
		ImVec2(g_dxWindow->GetScreenSize())
	);
}

void Menu::MainMenu()
{
    auto MenuRect = GetImGuiMenuBounds();
	ImGui::SetNextWindowPos(MenuRect.Min);
	ImGui::SetNextWindowSize(MenuRect.GetSize());
    if (!ImGui::Begin("OBLIVION REMASTERED", &bShowMenu, 96))
    {
        ImGui::End();
        return;
    }
    auto width = ImGui::GetContentRegionAvail().x;
    auto height = ImGui::GetContentRegionAvail().y;

    //  Full Bright
    if (ImGui::Checkbox("FULL BRIGHT", &g_Oblivion->bFullbright))
        TESOblivion::Fullbright(g_Oblivion->bFullbright);

    //  ESP
    ImGui::Checkbox("ESP", &this->bESP);
    if (this->bESP)
    {
        ImGui::SameLine();
        ImGui::Checkbox("##names", &this->bESPName);
        GUI::Tooltip("NAMES");
        ImGui::SameLine();
        ImGui::Checkbox("##snap_lines", &this->bESPSnap);
        GUI::Tooltip("SNAP LINES");

        ImGui::SameLine();
        ImGui::SetCursorPosX(width * .25);
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::SliderFloat("##ESP_DISTANCE", &this->mESPDist, 0.0f, 100.f, "%.0f");
    }

    ImGui::SetCursorPosY(height - ImGui::GetTextLineHeightWithSpacing() * 2);
    if (ImGui::Button("EXIT", ImGui::GetContentRegionAvail()))
    {
        //  shutdown
        this->bRunning = false;
    }

    ImGui::End();
}

void Menu::SHROUD()
{
    const ImRect& wndw = GetClientScreenBounds();
    ImGui::SetNextWindowPos(wndw.Min);
    ImGui::SetNextWindowSize(wndw.GetSize());
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
    if (!ImGui::Begin("##SHROUDWINDOW", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs))
    {
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        ImGui::End();
        return;
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::End();
}

void Menu::HUD()
{
    const ImRect& wndw = GetCloneOverlayBounds();
    ImGui::SetNextWindowPos(wndw.Min);
    ImGui::SetNextWindowSize(wndw.GetSize());
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
    if (!ImGui::Begin("##HUDWINDOW", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs))
    {
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        ImGui::End();
        return;
    }
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar();

    ImDrawList* ImDraw = ImGui::GetWindowDrawList();
    auto center = wndw.GetCenter();
    auto top_center = ImVec2({ center.x, 0.f });
    GUI::DrawTextCentered(top_center + wndw.Min, IM_COL32_WHITE, "https://www.unknowncheats.me/", 24.f);


    TESOblivion::SGlobals cache = g_Oblivion->GetCache();

    //  ESP
    if (this->bESP)
    {
        for (TESOblivion::SImGuiActor& actor : cache.render.actors)
        {
            UnrealEngine::FVector2D screen;
            if (!UnrealEngine::Tools::TransformWorldToScreen(
                cache.CameraView, 
                actor.TM.pos, 
                screen, 
                { wndw.GetSize().x, wndw.GetSize().y }, 
                true
            ))
                continue;

            ImVec2 pos = ImVec2(screen.X, screen.Y) + wndw.Min;

            auto dist3D = cache.CameraView.POV.Location.Distance(actor.TM.pos);
            if (dist3D > this->mESPDist)
                continue;

            if (this->bESPSnap)
                GUI::CleanLine(wndw.GetSize() + wndw.Min, pos, IM_COL32_WHITE, 1.0f);
            
            if (this->bESPName)
                GUI::DrawTextCentered(pos, IM_COL32_WHITE, actor.name.c_str());
        }
    }

    ImGui::End();
}

DxWindow::SOverlay Menu::GetOverlay() { return elements; }

void Menu::UpdateOverlayViewState(bool bState) { elements.bIsShown = bState; }

void GUI::TextCentered(const char* pText)
{
    ImVec2 textSize = ImGui::CalcTextSize(pText);
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 textPos = ImVec2((windowSize.x - textSize.x) * 0.5f, (windowSize.y - textSize.y) * 0.5f);
    ImGui::SetCursorPos(textPos);
    ImGui::Text("%s", pText);
}

void GUI::Tooltip(const char* tip)
{
    if (!ImGui::IsItemHovered())
        return;
    
    ImGui::SetTooltip(tip);
}

void GUI::DrawText_(const ImVec2& pos, const ImColor& color, const std::string& text, const float& szFont)
{
	ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), szFont, pos, color, text.c_str(), text.c_str() + text.size(), 800.f, nullptr);
}

void GUI::DrawTextCentered(const ImVec2& pos, const ImColor& color, const std::string& text, const float& szFont)
{
    const ImVec2& textSize = ImGui::CalcTextSize(text.c_str());
    ImVec2 textPosition = ImVec2(pos.x - (textSize.x * 0.5f), pos.y);
    if (szFont <= 0.f)
    {
        DrawText_(textPosition, color, text, szFont);
        return;
    }

    auto pFont = ImGui::GetFont();
    ImVec2 scaledTextSize = ImVec2(textSize.x * szFont / pFont->FontSize, szFont);
    ImVec2 scaledTextPos = ImVec2(pos.x - (scaledTextSize.x * .5f), pos.y);
    DrawText_(scaledTextPos, color, text, szFont);
}

void GUI::Line(const ImVec2& posA, const ImVec2& posB, const ImColor& color, const float& thickness)
{
    ImGui::GetWindowDrawList()->AddLine(posA, posB, color, thickness);
}

void GUI::Circle(const ImVec2& pos, const ImColor& color, const float& radius, const float& thickness, const float& segments)
{
    ImGui::GetWindowDrawList()->AddCircle(pos, radius, color, segments, thickness);
}

void GUI::CleanLine(const ImVec2& posA, const ImVec2& posB, const ImColor& color, const float& thickness)
{
    Line(posA, posB, ImColor(0.0f, 0.0f, 0.0f, color.Value.w), (thickness + 0.25));
    Line(posA, posB, ImColor(1.0f, 1.0f, 1.0f, color.Value.w), (thickness + 0.15));
    Line(posA, posB, color, thickness);
}

void GUI::CleanCircle(const ImVec2& pos, const ImColor& color, const float& radius, const float& thickness, const float& segments)
{
    Circle(pos, ImColor(0.0f, 0.0f, 0.0f, color.Value.w), radius, thickness, segments);
    Circle(pos, ImColor(1.0f, 1.0f, 1.0f, color.Value.w), radius, thickness, segments);
    Circle(pos, color, radius, thickness, segments);
}