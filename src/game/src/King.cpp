#include <algorithm>
#include <App.h>
#include <game/PawnColor.h>
#include <game/King.h>
#include <vector>

King::King(PawnColor color) : Pawn(color) {
  Pawn::setTexture("king");
  this->steps.push_back(std::make_pair<int, int>(1, 1));
  this->steps.push_back(std::make_pair<int, int>(-1, 1));
  this->steps.push_back(std::make_pair<int, int>(1, -1));
  this->steps.push_back(std::make_pair<int, int>(-1, -1));
  this->steps.push_back(std::make_pair<int, int>(1, 0));
  this->steps.push_back(std::make_pair<int, int>(-1, 0));
  this->steps.push_back(std::make_pair<int, int>(0, 1));
  this->steps.push_back(std::make_pair<int, int>(0, -1));
}

std::vector<std::pair<int, int>> King::getAdvanceableSquares() {
  auto squares{Pawn::getAdvanceableSquares(this->steps, King::maxSteps, false)};
  auto kingSquares{this->getKingSquares(squares)};
  auto castlingSquares{this->castle()};
  kingSquares.insert(kingSquares.end(), castlingSquares.begin(), castlingSquares.end());

  return kingSquares;
}

std::vector<std::pair<int, int>> King::getHoldingSquares() {
  auto squares { Pawn::getAdvanceableSquares(this->steps, King::maxSteps, true) };

  return this->getKingSquares(squares);
}

std::vector<std::pair<int, int>> King::getKingSquares(std::vector<std::pair<int, int>> &pairs) const {
  std::erase_if(pairs, [this](const std::pair<int, int> &pair) {
    return static_cast<App *>(graphics::getUserData())
               ->getGame()
               .lock()
               ->getCheckboard()
               ->getSquare(pair)
               .lock()
               ->getDangerReferenceCount(!Pawn::getColor()) > 0;
  });

  return pairs;
}

bool King::isChecked() const {
  const PawnColor oppositeColor{!Pawn::getColor()};

  bool result;

  if (Pawn::getSquare().expired()) {
    result = {false};
  } else {
    result = {Pawn::getSquare().lock()->getDangerReferenceCount(oppositeColor) > 0};
  }

  return result;
}

std::vector<std::pair<int, int>> King::castle() {
  auto castlingMoves{std::vector<std::pair<int, int>>()};

  if (!this->hasMoved) {
    const auto checkboard{static_cast<App *>(graphics::getUserData())
                              ->getGame()
                              .lock()
                              ->getCheckboard()};
    if (!this->leftRookHasMoved) {
      const auto nextLeftSquare{std::make_pair(Pawn::getSquare().lock()->getRow(), 3)};
      auto secondNextLeftSquare{std::make_pair(Pawn::getSquare().lock()->getRow(), 2)};
      const auto thirdNextLeftSquare{std::make_pair(Pawn::getSquare().lock()->getRow(), 1)};
      const bool nextEmptyAndSafe{(!checkboard->getSquare(nextLeftSquare).lock()->hasPawn()) && (0 == checkboard->getSquare(nextLeftSquare).lock()->getDangerReferenceCount(!Pawn::getColor()))};
      const bool secondNextEmptyAndSafe{(!checkboard->getSquare(secondNextLeftSquare).lock()->hasPawn()) && (0 == checkboard->getSquare(secondNextLeftSquare).lock()->getDangerReferenceCount(!Pawn::getColor()))};
      const bool thirdNextEmpty{!checkboard->getSquare(thirdNextLeftSquare).lock()->hasPawn()};

      if (nextEmptyAndSafe && secondNextEmptyAndSafe && thirdNextEmpty && (!this->isChecked())) {
        castlingMoves.emplace_back(secondNextLeftSquare);
      }
    }

    if (!this->rightRookHasMoved) {
      const auto nextRightSquare{std::make_pair(Pawn::getSquare().lock()->getRow(), 5)};
      auto secondNextRightSquare{std::make_pair(Pawn::getSquare().lock()->getRow(), 6)};
      const bool nextEmptyAndSafe{(!checkboard->getSquare(nextRightSquare).lock()->hasPawn()) && (0 == checkboard->getSquare(nextRightSquare).lock()->getDangerReferenceCount(!Pawn::getColor()))};
      const bool secondNextEmptyAndSafe{(!checkboard->getSquare(secondNextRightSquare).lock()->hasPawn()) && (0 == checkboard->getSquare(secondNextRightSquare).lock()->getDangerReferenceCount(!Pawn::getColor()))};

      if (nextEmptyAndSafe && secondNextEmptyAndSafe && (!this->isChecked())) {
        castlingMoves.emplace_back(secondNextRightSquare);
      }
    }
  }

  return castlingMoves;
}

void King::setMoved() {
  this->hasMoved = {true};
}

void King::setRookMoved(const int column) {
  if (0 == column) {
    this->leftRookHasMoved = {true};
  } else {
    this->rightRookHasMoved = {true};
  }
}

bool King::moved() const {
  return this->hasMoved;
}
