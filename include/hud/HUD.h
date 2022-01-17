
#ifndef HUD_H
#define HUD_H

#include "graphics/Brush.h"
#include "graphics/Drawable.h"
#include "graphics/Rectangle.h"
#include "graphics/Updateable.h"
#include <optional>
//#ifdef _WIN32
//  #include "sgg/sgg/graphics.h"
//#else
//  #include <sgg/graphics.h>
//#endif
#include "sgg/sgg/graphics.h"
#include <string>

class HUD : public Drawable, public Updateable {
  std::string message{};
  std::pair<float, float> coordinates{};
  std::size_t size{};
  Rectangle drawingArea{Rectangle()};
  std::pair<bool, std::optional<std::string>> newMessage{std::make_pair(false, std::optional<std::string>())};

public:
  HUD();
  explicit HUD(std::string message, std::pair<float, float> coordinates, size_t size);
  void draw() override;
  void update(float ms) override;
  void updateText(std::string text);
  [[nodiscard]] const std::pair<float, float> &getCoordinates() const;
  [[nodiscard]] size_t getSize() const;
  [[nodiscard]] float getLength() const;
  void setTextBrush(Brush type);
};

#endif// HUD_H
