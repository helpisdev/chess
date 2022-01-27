#include <graphics/MenuOption.h>
#include <utility>

MenuOption::MenuOption(HUD hud) : optionHUD(std::move(hud)) {
  const auto [buttonX, buttonY] { this->optionHUD.getCoordinates() };
  const std::size_t size{this->optionHUD.getSize()};
  const float length{(this->optionHUD.getLength() * static_cast<float>(size) / 2) + 200.0f};
  this->button.setHeight(static_cast<float>(size) * 1.5f);
  this->button.setWidth(length);
  this->button.setLeftBottom(Point(buttonX - 50.0f, buttonY + (static_cast<float>(size) / 2)));
  this->button.setBrush(Brush::DARK_BROWN, Rectangle::colors.at(Brush::DARK_BROWN));
}

HUD MenuOption::getMessage() const {
  return this->optionHUD;
}

Rectangle MenuOption::getButton() const {
  return this->button;
}

void MenuOption::playSound() const {
  this->clickSound.play();
}
