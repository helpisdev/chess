
#ifndef KING_H
#define KING_H

#include "Pawn.h"
#include "Checkboard.h"

class King : public Pawn {
  std::vector<std::pair<int, int>> steps{std::vector<std::pair<int, int>>()};
  static constexpr unsigned short int maxSteps{1};
  bool hasMoved{false};
  bool rightRookHasMoved{false};
  bool leftRookHasMoved{false};

public:
  explicit King(PawnColor color);
  std::vector<std::pair<int, int>> getAdvanceableSquares() override;
  std::vector<std::pair<int, int>> getHoldingSquares() override;
  std::vector<std::pair<int, int>> getKingSquares(std::vector<std::pair<int, int>> &pairs) const;
  [[nodiscard]] bool isChecked() const;
  std::vector<std::pair<int, int>> castle();
  void setMoved();
  void setRookMoved(int column);
  [[nodiscard]] bool moved() const;
};

#endif // KING_H
