#include <game/Soldier.h>
#include <App.h>
#include <algorithm>

Soldier::Soldier(PawnColor color) : Pawn(color) {
  Pawn::setTexture("soldier");
}

std::vector<std::pair<int, int>> Soldier::getAdvanceableSquares() {
  std::vector<std::pair<int, int>> steps{std::vector<std::pair<int, int>>()};
  auto checkboard{static_cast<App *>(graphics::getUserData())->getGame().lock()->getCheckboard()};

  steps.push_back(std::make_pair<int, int>(1, 0));
  steps.push_back(std::make_pair<int, int>(1, 1));
  steps.push_back(std::make_pair<int, int>(1, -1));

  if (const int nextRowWhite{this->getSquare().lock()->getRow() + 1}; nextRowWhite <= 7) {
    const bool startingPositionWhite{this->getSquare().lock()->getRow() == 1};
    const bool pawnAheadWhite{checkboard->getSquareInfo(std::make_pair(nextRowWhite, this->getSquare().lock()->getColumn())).first};
    if (startingPositionWhite && Pawn::getColor() == PawnColor::WHITE && !pawnAheadWhite) {
      steps.push_back(std::make_pair<int, int>(2, 0));
    }
  }

  if (const int nextRowBlack{this->getSquare().lock()->getRow() - 1}; nextRowBlack >= 0) {
    const bool startingPositionBlack{this->getSquare().lock()->getRow() == 6};
    const bool pawnAheadBlack{checkboard->getSquareInfo(std::make_pair(nextRowBlack, this->getSquare().lock()->getColumn())).first};
    if (startingPositionBlack && Pawn::getColor() == PawnColor::BLACK && !pawnAheadBlack) {
      steps.push_back(std::make_pair<int, int>(2, 0));
    }
  }

  constexpr unsigned short int maxSteps{1};

  auto advanceableSquares{Pawn::getAdvanceableSquares(steps, maxSteps, false)};

  std::erase_if(advanceableSquares, [&checkboard, this](const auto &pair) {
    if (pair.second != this->getSquare().lock()->getColumn()) {
      const auto &[hasPawn, pawnColor]{checkboard->getSquareInfo(pair)};
      return !hasPawn;
    }

    if (pair.second == this->getSquare().lock()->getColumn()) {
      const auto &[hasPawn, pawnColor]{checkboard->getSquareInfo(pair)};
      return hasPawn;
    }

    return false;
  });

  return advanceableSquares;
}

std::vector<std::pair<int, int>> Soldier::getHoldingSquares() {
  std::vector<std::pair<int, int>> steps { std::vector<std::pair<int, int>>() };

  steps.push_back(std::make_pair<int, int>(1, 1));
  steps.push_back(std::make_pair<int, int>(1, -1));

  constexpr unsigned short int maxSteps { 1 };

  auto advanceableSquares { Pawn::getAdvanceableSquares(steps, maxSteps, true) };

  std::erase_if(advanceableSquares, [this](const auto& pair) {
    if (pair.second == this->getSquare().lock()->getColumn()) {
      auto checkboard{static_cast<App *>(graphics::getUserData())->getGame().lock()->getCheckboard()};
      const auto &[hasPawn, pawnColor]{checkboard->getSquareInfo(pair)};
      return hasPawn;
    }

    return false;
  });

  return advanceableSquares;
}
