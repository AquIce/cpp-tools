#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <typeinfo>


namespace Redstone {
    class Component {
        public:
            std::string type;
            int intensity;
            Component* up;
            Component* down;
            Component* left;
            Component* right;
			int direction = 0;
            
            virtual bool is_linkable(Component* other, int side) = 0;
			virtual void spread(int intensity) {}
    };

    class Powder : public Component {
        public:
            Powder() {
                type = "POWDER";
                intensity = 0;
                up = nullptr;
                down = nullptr;
                left = nullptr;
                right = nullptr;
            }
            bool is_linkable(Component* other, int side) {
                if(other->type == "POWDER")
                    return true;
				else if(other->type == "REPEATER" && other->direction == side)
					return true;
                else
                    return false;
            }
			void spread(int intensity) {
				this->intensity = intensity;
				if(this->intensity > 1) {
					if(this->up != nullptr) {
						if(this->intensity - 1 > this->up->intensity)
							this->up->spread(this->intensity - 1);
					}
					if(this->down != nullptr) {
						if(this->intensity - 1 > this->down->intensity)
							this->down->spread(this->intensity - 1);
					}
					if(this->left != nullptr) {
						if(this->intensity - 1 > this->left->intensity)
							this->left->spread(this->intensity - 1);
					}
					if(this->right != nullptr) {
						if(this->intensity - 1 > this->right->intensity)
							this->right->spread(this->intensity - 1);
					}
				}
			}
    };

	class Repeater : public Component {
		public:
            Repeater(int orientation) {
                type = "REPEATER";
                intensity = 0;
                up = nullptr;
                down = nullptr;
                left = nullptr;
                right = nullptr;
				direction = orientation;
			}
            bool is_linkable(Component* other, int side) {
				if(side == direction) {
	                if(other->type == "POWDER")
	                    return true;
					else if(other->type == "REPEATER" && other->direction == side)
						return true;
				}
                return false;
            }
			void spread(int intensity) {
				this->intensity = 15;
				if(direction == 0) {
					if(this->up != nullptr)
						this->up->spread(this->intensity);
				}
			}
	};
}

template <int v, int h>
class PlayGround {
    private:
        std::array<std::array<Redstone::Component*, h>, v> grid;

        bool in(std::vector<std::array<int, 2>> coords, int x, int y) {
            for(int i = 0; i < coords.size(); i++) {
                if(coords[i][0] == x && coords[i][1] == y)
                    return true;
            }
            return false;
        }

    public:
        PlayGround(std::vector<std::array<int, 2>> powders, std::vector<std::array<int, 2>> repeaters) {
            for(int i = 0; i < v; i++) {
                for(int j = 0; j < h; j++) {
                    if(in(powders, i, j))
                        grid[i][j] = new Redstone::Powder();
					else if(in(repeaters, i, j))
						grid[i][j] = new Redstone::Repeater(3);
                    else
                        grid[i][j] = nullptr;
                }
            }
        }

        void link() {
            for(int i = 0; i < v; i++) {
                for(int j = 0; j < h; j++) {
                    if(grid[i][j] == nullptr)
                        continue;
                    // UP
                    if(i == 0)
                        grid[i][j]->up = nullptr;
                    else if(grid[i - 1][j] == nullptr || !grid[i - 1][j]->is_linkable(grid[i][j], 0))
                        grid[i][j]->up = nullptr;
                    else
                        grid[i][j]->up = grid[i - 1][j];
                    // DOWN
                    if(i == v - 1)
                        grid[i][j]->down = nullptr;
                    else if(grid[i + 1][j] == nullptr || !grid[i + 1][j]->is_linkable(grid[i][j], 1))
                        grid[i][j]->down = nullptr;
                    else
                        grid[i][j]->down = grid[i + 1][j];
                    // LEFT
                    if(j == 0)
                        grid[i][j]->left = nullptr;
                    else if(grid[i][j - 1] == nullptr || !grid[i][j - 1]->is_linkable(grid[i][j], 2))
                        grid[i][j]->left = nullptr;
                    else
                        grid[i][j]->left = grid[i][j - 1];
                    // RIGHT
                    if(j == h - 1)
                        grid[i][j]->right = nullptr;
                    else if(grid[i][j + 1] == nullptr || !grid[i][j + 1]->is_linkable(grid[i][j], 3))
                        grid[i][j]->right = nullptr;
                    else
                        grid[i][j]->right = grid[i][j + 1];
                }
            }
        }

		void power(int x, int y) {
			grid[x][y]->spread(15);
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
			std::cout << "Intensity : " << grid[pos[0]][pos[1]]->intensity << std::endl;
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
    PlayGround<20, 20> pg = PlayGround<20, 20>({{0, 0}, {0, 2}}, {{0, 1}});
    pg.link();
	pg.power(0, 0);
    pg.print();
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 20; j++) {
            std::cout << "{" << i << ", " << j << "}" << std::endl;
            pg.print({i, j});
        }
    }
    return 0;
}