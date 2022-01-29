#include <game/Queen.h>
#include <App.h>

Queen::Queen(PawnColor color) : Pawn(color) {
  Pawn::setTexture("queen");
  this->steps.push_back(std::make_pair<int, int>(1, 1));
  this->steps.push_back(std::make_pair<int, int>(-1, 1));
  this->steps.push_back(std::make_pair<int, int>(1, -1));
  this->steps.push_back(std::make_pair<int, int>(-1, -1));
  this->steps.push_back(std::make_pair<int, int>(1, 0));
  this->steps.push_back(std::make_pair<int, int>(-1, 0));
  this->steps.push_back(std::make_pair<int, int>(0, 1));
  this->steps.push_back(std::make_pair<int, int>(0, -1));
}

std::vector<std::pair<int, int>> Queen::getAdvanceableSquares() {
  return Pawn::getAdvanceableSquares(this->steps, Queen::maxSteps, false);
}

std::vector<std::pair<int, int>> Queen::getHoldingSquares() {
  return Pawn::getAdvanceableSquares(this->steps, Queen::maxSteps, true);
}

std::weak_ptr<Pawn> Queen::getBlockedPawn() {
  auto game{static_cast<App *>(graphics::getUserData())->getGame().lock()};
  auto checkboard{game->getCheckboard()};

  const unsigned short int currentRow{this->getSquare().lock()->getRow()};
  const unsigned short int currentColumn{this->getSquare().lock()->getColumn()};

  unsigned short int nextRow{currentRow};
  unsigned short int nextColumn{currentColumn};
  for (const auto &[rowStep, columnStep]: Pawn::correctDirection(this->steps)) {
    bool shouldTerminate{false};
    bool foundPawn{false};
    std::shared_ptr<Pawn> pawn{nullptr};

    unsigned short int numberOfSteps{0};
    while (!shouldTerminate) {
      nextRow += rowStep;
      nextColumn += columnStep;
      if ((nextRow >= Checkboard::sideSize || nextColumn >= Checkboard::sideSize) || numberOfSteps == Queen::maxSteps) {
        shouldTerminate = { true };
        continue;
      }
      ++numberOfSteps;

      std::pair<int, int> squareCoordinates { std::make_pair<int, int>(nextRow, nextColumn) };
      auto [hasPawn, pawnColor] { checkboard->getSquareInfo(squareCoordinates) };

      if (hasPawn && pawnColor != Pawn::getColor() && foundPawn && checkboard->isKing(pawn->getColor(), squareCoordinates)) {
        return pawn;
      }

      if (hasPawn && pawnColor != Pawn::getColor() && foundPawn) {
        shouldTerminate = { true };
      }

      if (hasPawn && pawnColor != Pawn::getColor()) {
        pawn = {checkboard->getSquare(squareCoordinates).lock()->getPawn()};
        foundPawn = {true};
      }
    }

    nextRow = { currentRow };
    nextColumn = { currentColumn };
  }

  return std::weak_ptr<Pawn>();
}
