#include <graphics/PlayingScreen.h>
#include <util/custom-string.h>
#include <hud/PawnCount.h>

PlayingScreen::PlayingScreen() {
  constexpr float squareWidth { 75.0f };
  for (unsigned short int white { 0 }; white < PlayingScreen::numberOfUniquePawns; ++white) {
    const Point leftBottom { Point(325.0f, 150.0f + static_cast<float>(white) * 75.0f) };
    (*this->capturedWhiteSquares)[white].first = { std::make_shared<Square>() };
    (*this->capturedWhiteSquares)[white].first->initialize(leftBottom, Rectangle::colors.at(Brush::TEXTURE), Brush::TEXTURE, squareWidth);
    const std::pair<float, float> coordinates { (*this->capturedWhiteSquares)[white].first->getDrawingArea()->getCenter().getX() - 75.0f,
                                              (*this->capturedWhiteSquares)[white].first->getDrawingArea()->getCenter().getY() + 25.0f };
    (*this->capturedWhiteSquares)[white].second = { std::make_pair(0, std::make_shared<PawnCount>(coordinates, squareWidth - 10.0f)) };
  }

  for (unsigned short int black { 0 }; black < PlayingScreen::numberOfUniquePawns; ++black) {
    const Point leftBottom { Point(325.0f, 900.0f - static_cast<float>(black) * 75.0f) };
    (*this->capturedBlackSquares)[black].first = { std::make_shared<Square>() };
    (*this->capturedBlackSquares)[black].first->initialize(leftBottom, Rectangle::colors.at(Brush::TEXTURE), Brush::TEXTURE, squareWidth);
    const std::pair<float, float> coordinates { (*this->capturedBlackSquares)[black].first->getDrawingArea()->getCenter().getX() - 75.0f,
                                              (*this->capturedBlackSquares)[black].first->getDrawingArea()->getCenter().getY() + 25.0f };
    (*this->capturedBlackSquares)[black].second = { std::make_pair(0, std::make_shared<PawnCount>(coordinates, squareWidth - 10.0f)) };
  }
}

void PlayingScreen::draw() {
  this->game->draw();
  for (unsigned short int i { 0 }; i < PlayingScreen::numberOfUniquePawns; ++i) {
    if ((*this->capturedWhiteSquares)[i].second.first != 0) {
      (*this->capturedWhiteSquares)[i].first->draw();
      (*this->capturedWhiteSquares)[i].second.second->draw();
    }

    if ((*this->capturedBlackSquares)[i].second.first != 0) {
      (*this->capturedBlackSquares)[i].first->draw();
      (*this->capturedBlackSquares)[i].second.second->draw();
    }
  }
}

void PlayingScreen::update(float ms) {
  this->game->update(ms);
  for (unsigned short int i { 0 }; i < PlayingScreen::numberOfUniquePawns; ++i) {
    if ((*this->capturedWhiteSquares)[i].second.first != 0) {
      (*this->capturedWhiteSquares)[i].first->update(ms);
      if ((*this->capturedWhiteSquares)[i].second.first > 0) {
        (*this->capturedWhiteSquares)[i].second.second->updateText(std::to_string((*this->capturedWhiteSquares)[i].second.first));
        (*this->capturedWhiteSquares)[i].second.second->update(ms);
      }
    }

    if ((*this->capturedBlackSquares)[i].second.first != 0) {
      (*this->capturedBlackSquares)[i].first->update(ms);
      if ((*this->capturedBlackSquares)[i].second.first > 0) {
        (*this->capturedBlackSquares)[i].second.second->updateText(std::to_string((*this->capturedBlackSquares)[i].second.first));
        (*this->capturedBlackSquares)[i].second.second->update(ms);
      }
    }
  }
}

std::shared_ptr<Square> PlayingScreen::getSquare(const std::shared_ptr<Pawn>& pawn) const {
  const std::string& texture { pawn->getTexture() };
  std::vector<std::string> directories { custom_string::split(texture, std::to_string(std::filesystem::path::preferred_separator)) };
  std::string pawnType { custom_string::split(directories.at(directories.size() - 1), "-").at(0) };
  std::size_t index;
  const PawnColor color { pawn->getColor() };

  switch (stringHash(pawnType.c_str())) {
    case "soldier"_hash:
      index = { 0 };
      break;
    case "rook"_hash:
      index = { 1 };
      break;
    case "knight"_hash:
      index = { 2 };
      break;
    case "bishop"_hash:
      index = { 3 };
      break;
    case "queen"_hash:
      index = { 4 };
      break;
    default:
      index = { 5 };
      // king
      break;
  }

  std::shared_ptr<Square> square { nullptr };
  if (color == PawnColor::WHITE) {
    square = { (*this->capturedWhiteSquares)[index].first };
    ++(*this->capturedWhiteSquares)[index].second.first;
  } else {
    square = { (*this->capturedBlackSquares)[index].first };
    ++(*this->capturedBlackSquares)[index].second.first;
  }

  if (square->hasPawn()) {
    return nullptr;
  }

  graphics::Brush textureBrush { Rectangle::colors.at(Brush::TEXTURE) };
  textureBrush.texture = { paths::getImagesPath() + texture + "-" + color + ".png" };

  square->initialize(square->getDrawingArea()->getLeftBottom(), textureBrush, Brush::TEXTURE, square->getDrawingArea()->getWidth());

  return square;
}
