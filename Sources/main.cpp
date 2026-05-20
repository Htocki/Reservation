#include <string>
#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

struct Item {
  std::string group_name;
  std::string name;
  unsigned int ordered_quantity;
  unsigned int required_quantity;
};

void PrintLine(
  std::string arrow_button_name,
  ImVec4 color,
  unsigned int& quantity,
  std::string text)
{
  float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
  ImGui::TextColored(color, text.c_str());
  ImGui::SameLine();
  ImGui::TextColored(color, "%d", quantity);
  ImGui::SameLine();
  ImGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat, true);
  if (ImGui::ArrowButton(("##left_" + arrow_button_name).c_str(), ImGuiDir_Left)) { if (quantity > 0) { quantity--; } }
  ImGui::SameLine(0.0f, spacing);
  if (ImGui::ArrowButton(("##right_" + arrow_button_name).c_str(), ImGuiDir_Right)) { quantity++; }
  ImGui::PopItemFlag();
}

void PrintItem(Item& item) {
  ImGui::AlignTextToFramePadding();
  ImGui::SeparatorText(item.name.c_str());

  PrintLine(item.name + "_ordered", ImVec4(1.0f, 1.0f, 0.0f, 1.0f), item.ordered_quantity, "Zakazat:");
  PrintLine(item.name + "_required", ImVec4(1.0f, 0.0f, 1.0f, 1.0f), item.required_quantity, "Trebuetsya:");
}

int main() {
  static Item item;
  item.name = "Pantum m2500w";
  item.ordered_quantity = 0;
  item.required_quantity = 6;

  sf::RenderWindow window(sf::VideoMode({ 400, 800 }), "Reservation");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->Clear();
  io.Fonts->AddFontFromFileTTF("Fonts/ProggyClean.ttf", 13, NULL, io.Fonts->GetGlyphRangesCyrillic());
  ImGui::SFML::UpdateFontTexture();

  static bool no_titlebar = true;
  static bool no_scrollbar = false;
  static bool no_menu = true;
  static bool no_move = true;
  static bool no_resize = true;
  static bool no_collapse = true;
  static bool no_close = false;
  static bool no_nav = false;
  static bool no_background = true;
  static bool no_bring_to_front = false;
  static bool unsaved_document = false;
  bool* p_open = nullptr;
    
  ImGuiWindowFlags window_flags = 0;
  if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
  if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
  if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
  if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
  if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
  if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
  if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
  if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
  if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
  if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
  if (no_close)           p_open = nullptr;

  sf::Clock deltaClock;
  while (window.isOpen()) {
    while (const auto event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    ImGui::SFML::Update(window, deltaClock.restart());
    
    ImGui::Begin("Reservation", p_open, window_flags);
      if (ImGui::CollapsingHeader("Teflony")) { PrintItem(item); }
    ImGui::End();

    //ImGui::ShowDemoWindow();

    window.clear();
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
}