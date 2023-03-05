#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <typeinfo>


namespace Redstone {
    class Redstone {
        public:
            std::string type;
            int intensity;
            Redstone* up;
            Redstone* down;
            Redstone* left;
            Redstone* right;
            
            virtual bool is_linkable(std::string T) = 0;
    };

    class Powder : public Redstone {
        public:
            Powder() {
                type = "POWDER";
                intensity = 0;
                up = nullptr;
                down = nullptr;
                left = nullptr;
                right = nullptr;
            }
            bool is_linkable(std::string T) {
                if(T == "POWDER")
                    return true;
                else
                    return false;
            }
    };
}

template <int v, int h>
class PlayGround {
    private:
        // Cross of powders (20 x 20)
        std::array<std::array<Redstone::Redstone*, h>, v> grid;

        bool in(std::vector<std::array<int, 2>> powders, int x, int y) {
            for(int i = 0; i < powders.size(); i++) {
                if(powders[i][0] == x && powders[i][1] == y)
                    return true;
            }
            return false;
        }

    public:
        PlayGround(std::vector<std::array<int, 2>> powders) {
            for(int i = 0; i < v; i++) {
                for(int j = 0; j < h; j++) {
                    if(in(powders, i, j))
                        grid[i][j] = new Redstone::Powder();
                    else
                        grid[i][j] = nullptr;
                }
            }
        }

        void link_powders() {
            for(int i = 0; i < v; i++) {
                for(int j = 0; j < h; j++) {
                    if(grid[i][j] == nullptr)
                        continue;
                    // UP
                    if(i == 0)
                        grid[i][j]->up = nullptr;
                    else if(grid[i - 1][j] == nullptr || !grid[i - 1][j]->is_linkable("POWDER"))
                        grid[i][j]->up = nullptr;
                    else
                        grid[i][j]->up = grid[i - 1][j];
                    // DOWN
                    if(i == v - 1)
                        grid[i][j]->down = nullptr;
                    else if(grid[i + 1][j] == nullptr || !grid[i + 1][j]->is_linkable("POWDER"))
                        grid[i][j]->down = nullptr;
                    else
                        grid[i][j]->down = grid[i + 1][j];
                    // LEFT
                    if(j == 0)
                        grid[i][j]->left = nullptr;
                    else if(grid[i][j - 1] == nullptr || !grid[i][j - 1]->is_linkable("POWDER"))
                        grid[i][j]->left = nullptr;
                    else
                        grid[i][j]->left = grid[i][j - 1];
                    // RIGHT
                    if(j == h - 1)
                        grid[i][j]->right = nullptr;
                    else if(grid[i][j + 1] == nullptr || !grid[i][j + 1]->is_linkable("POWDER"))
                        grid[i][j]->right = nullptr;
                    else
                        grid[i][j]->right = grid[i][j + 1];
                }
            }
        }

        void print() {
            for(int i = 0; i < v; i++) {
                for(int j = 0; j < h; j++) {
                    if(grid[i][j] == nullptr)
                        std::cout << "-";
                    else
                        std::cout << "X";
                }
                std::cout << std::endl;
            }
        }

        void print(std::array<int, 2> pos) {
            if(grid[pos[0]][pos[1]] == nullptr) {
                std::cout << "-" << std::endl;
                return;
            }
            if(grid[pos[0]][pos[1]]->up != nullptr)
                std::cout << "Linked UP" << std::endl;
            if(grid[pos[0]][pos[1]]->down != nullptr)
                std::cout << "Linked DOWN" << std::endl;
            if(grid[pos[0]][pos[1]]->left != nullptr)
                std::cout << "Linked LEFT" << std::endl;
            if(grid[pos[0]][pos[1]]->right != nullptr)
                std::cout << "Linked RIGHT" << std::endl;
        }
};

int main() {
    PlayGround pg = PlayGround<20, 20>({{0, 0}, {0, 1}, {0, 2}, {1, 1}, {1, 2}, {2, 1}, {2, 2}, {3, 1}, {3, 2}, {4, 1}, {4, 2}, {5, 1}, {5, 2}, {6, 1}, {6, 2}, {7, 1}, {7, 2}, {8, 1}, {8, 2}, {9, 1}, {9, 2}, {10, 1}, {10, 2}, {11, 1}, {11, 2}, {12, 1}, {12, 2}, {13, 1}, {13, 2}, {14, 1}, {14, 2}, {15, 1}, {15, 2}, {16, 1}, {16, 2}, {17, 1}, {17, 2}, {18, 1}, {18, 2}, {19, 1}, {19, 2}});
    pg.link_powders();
    pg.print();
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 20; j++) {
            std::cout << "{" << i << ", " << j << "}" << std::endl;
            pg.print({i, j});
        }
    }
    return 0;
}