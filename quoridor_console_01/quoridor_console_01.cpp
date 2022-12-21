#include <iostream>
#include <vector>
#include <Windows.h>
using namespace std;

static int player_1_global_score = 0, player_2_global_score = 0;

enum Direction
{
	up = 0, down, right, left
};

enum WallLocation
{
	horyzontally = 0, vertically
};

class QuoridorGamePlayer
{
private:

	int technical_grid_size;

	int pos_x;
	int pos_y;
	int walls_left;
	int game_moves = 0;

public:

	QuoridorGamePlayer(int pos_x, int pos_y, int grid_size)
	{
		walls_left = grid_size + 1;

		if (pos_x % 2 != 0)
			pos_x--;

		if (pos_y % 2 != 0)
			pos_y--;

		technical_grid_size = grid_size * 2 - 1;

		if (pos_x < 0 || pos_x > technical_grid_size - 1 || pos_y < 0 || pos_y > technical_grid_size - 1)
		{
			this->pos_x = this->pos_y = 0;
		}
		else
		{
			this->pos_x = pos_x;
			this->pos_y = pos_y;
		}
	}

	int get_pos_x() { return pos_x; }
	int get_pos_y() { return pos_y; }
	int get_walls_number() { return walls_left; }
	int get_game_moves() { return game_moves; }

	void move(Direction dir)
	{
		switch (dir)
		{
		case 0:

			if (pos_y > 1)
			{
				pos_y -= 2;
				game_moves++;
			}
			break;

		case 1:

			if (pos_y < technical_grid_size - 1)
			{
				pos_y += 2;
				game_moves++;
			}
			break;

		case 2:

			if (pos_x < technical_grid_size - 1)
			{
				pos_x += 2;
				game_moves++;
			}
			break;

		case 3:

			if (pos_x > 1)
			{
				pos_x -= 2;
				game_moves++;
			}
			break;

		default:
			break;
		}
	}

	void use_wall()
	{
		if (walls_left <= 0)
			return;
		walls_left--;
		game_moves++;
	}
};

class QuoridorGame
{
private:

	vector<vector<bool>> walls_play_grid;
	int technical_grid_size;

public:

	QuoridorGamePlayer Player_1, Player_2;

	QuoridorGame(int grid_size) :
		Player_1((grid_size > 13 ? grid_size = 13 : grid_size < 5 ? grid_size = 5 : grid_size - 1), 0, (grid_size)) ,
		Player_2((grid_size - 1), (grid_size * 2 - 2), (grid_size))
	{
		walls_play_grid.resize(grid_size * 2 - 1);

		for (auto& i : walls_play_grid)
			i.resize(grid_size * 2 - 1);

		technical_grid_size = grid_size * 2 - 1;
	}

	void console_grid_output()
	{
		for (int i = -2; i < technical_grid_size; i++)
		{
			for (int j = -2; j < technical_grid_size; j++)
			{
				if (i == -2 && j % 2 != 0 && i != j && j != -1)
					cout << (j < 10 ? "0" : "") << j;

				else if (i % 2 != 0 && j == -2 && i != j && i != -1)
					cout << char(65 + i) << " ";

				else if ((i == Player_1.get_pos_y() && j == Player_1.get_pos_x()))
					cout << "P1";

				else if (i == Player_2.get_pos_y() && j == Player_2.get_pos_x())
					cout << "P2";

				else if (i % 2 == 0 && j % 2 == 0 && i >= 0 && j >= 0)
					cout << "[]";
				
				else if (i >= 0 && j >= 0 && walls_play_grid[i][j] == true)
					cout << "XX";

				else
					cout << "  ";
			}
			cout << "\n";
		}
	}

	void console_players_info_output()
	{
		cout << "\n(i) √равець 1:\n   —т≥ни -> " << Player_1.get_walls_number() << ";\n   ’оди -> " << Player_1.get_game_moves() << ";\n"
			<< ".\n\n(i) √равець 2:\n   —т≥ни -> " << Player_2.get_walls_number() << ";\n   ’оди -> " << Player_2.get_game_moves() << ";\n";
	}

	bool way_is_open(QuoridorGamePlayer& player, Direction dir)
	{
		switch (dir)
		{
		case 0:

			return player.get_pos_y() > 1 && walls_play_grid[player.get_pos_y() - 1][player.get_pos_x()] != true;

		case 1:

			return player.get_pos_y() < technical_grid_size - 1 && walls_play_grid[player.get_pos_y() + 1][player.get_pos_x()] != true;

		case 2:

			return player.get_pos_x() < technical_grid_size - 1 && walls_play_grid[player.get_pos_y()][player.get_pos_x() + 1] != true;

		case 3:

			return player.get_pos_x() > 1 && walls_play_grid[player.get_pos_y()][player.get_pos_x() - 1] != true;

		default:
			return false;
		}
	}

	bool way_is_open(int wall_x_pos, int wall_y_pos, WallLocation loc)
	{
		if(wall_x_pos > 0 && wall_x_pos < technical_grid_size && wall_y_pos > 0 && wall_y_pos < technical_grid_size)
		{
			if (loc == horyzontally && walls_play_grid[wall_y_pos][wall_x_pos - 1] != true && walls_play_grid[wall_y_pos][wall_x_pos + 1] != true)
				return true;

			else if (loc == vertically && walls_play_grid[wall_y_pos - 1][wall_x_pos] != true && walls_play_grid[wall_y_pos + 1][wall_x_pos] != true)
				return true;
		}

		return false;
	}

	void locate_wall(QuoridorGamePlayer& player, int wall_x_pos, int wall_y_pos, WallLocation loc)
	{
		if (player.get_walls_number() > 0 && wall_x_pos < technical_grid_size && wall_y_pos < technical_grid_size && wall_y_pos % 2 != 0 && wall_x_pos % 2 != 0)
		{
			if (loc == horyzontally && walls_play_grid[wall_y_pos][wall_x_pos - 1] != true && walls_play_grid[wall_y_pos][wall_x_pos + 1] != true)
			{
				player.use_wall();
				walls_play_grid[wall_y_pos][wall_x_pos] = true;
				walls_play_grid[wall_y_pos][wall_x_pos - 1] = true;
				walls_play_grid[wall_y_pos][wall_x_pos + 1] = true;
			}

			else if (loc == vertically && walls_play_grid[wall_y_pos - 1][wall_x_pos] != true && walls_play_grid[wall_y_pos + 1][wall_x_pos] != true)
			{
				player.use_wall();
				walls_play_grid[wall_y_pos][wall_x_pos] = true;
				walls_play_grid[wall_y_pos - 1][wall_x_pos] = true;
				walls_play_grid[wall_y_pos + 1][wall_x_pos] = true;
			}
		}
	}

};

void console_main_menu(int& user_console_chioce)
{
	while (true)
	{
		cout << "QUORIDOR\nввед≥ть:\n<1> - дл€ налаштуванн€ гри\n<2> - дл€ початку гри\n<3> - дл€ виходу з гри\n-> ";
		cin >> user_console_chioce;
		system("cls");
		if (user_console_chioce == 1 || user_console_chioce == 2 || user_console_chioce == 3)
			break;
	}
}

void console_settings_menu(int& user_console_chioce, int& grid_size)
{
	while (true)
	{
		cout << "ЌјЋјЎ“”¬јЌЌя\nрозм≥р пол€ дл€ гри: " << grid_size << " (за замовчуванн€м - 9)\n"
			"введ≥ть:\n<1> - дл€ зм≥ни розм≥ра пол€\n<2> - дл€ виходу з налаштувань\n-> ";
		cin >> user_console_chioce;
		system("cls");
		if (user_console_chioce == 1 || user_console_chioce == 2)
			break;
	}
	if (user_console_chioce == 2)
		return;

	cout << "ЌјЋјЎ“”¬јЌЌя\nввед≥ть новий розм≥р пол€\n(максимальне - 13; м≥н≥мальне - 5)\n(старе значенн€ - " << grid_size << ")\n-> ";
	cin >> grid_size;

	if (grid_size > 13)
		grid_size = 13;

	else if (grid_size < 5)
		grid_size = 5;

	system("cls");
	cout << "зм≥ни збережено.\n";
	system("pause");
	system("cls");
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int grid_size = 9;
	while(true)
	{
		int user_console_chioce;
		console_main_menu(user_console_chioce);
		if (user_console_chioce == 1)
		{
			console_settings_menu(user_console_chioce, grid_size);
			continue;
		}
		else if (user_console_chioce == 3)
		{
			cout << ":(";
			break;
		}
		else
		{
			int turn = 1;
			int user_game_choice;
			QuoridorGame GameProccess(grid_size);
			while (GameProccess.Player_1.get_pos_y() != grid_size * 2 - 2 && GameProccess.Player_2.get_pos_y() != 0)
			{
				system("cls");
				GameProccess.console_grid_output();
				GameProccess.console_players_info_output();
				system("pause");
				if (turn % 2 != 0)
				{
					while(true)
					{
						system("cls");
						GameProccess.console_grid_output();
						cout << "\nчерга √равц€ 1\n";
						if (GameProccess.Player_1.get_walls_number() > 0)
						{
							cout << "введ≥ть:\n<1> - дл€ перем≥щенн€\n<2> - дл€ розм≥щенн€ ст≥ни\n-> ";
							cin >> user_game_choice;
							if (user_game_choice == 1 || user_game_choice == 2)
								break;
						}
						else
						{
							cout << "введ≥ть:\n<1> - дл€ перем≥щенн€\n-> ";
							cin >> user_game_choice;
							if (user_game_choice == 1)
								break;
						}
					}
					if (user_game_choice == 1)
					{
						vector<int> possible_ways;

						if (GameProccess.way_is_open(GameProccess.Player_1, Direction::up))
							possible_ways.push_back(0);

						if (GameProccess.way_is_open(GameProccess.Player_1, Direction::down))
							possible_ways.push_back(1);

						if (GameProccess.way_is_open(GameProccess.Player_1, Direction::right))
							possible_ways.push_back(2);

						if (GameProccess.way_is_open(GameProccess.Player_1, Direction::left))
							possible_ways.push_back(3);

						while(true)
						{
							int choice;
							system("cls");
							GameProccess.console_grid_output();
							cout << "\nчерга √равц€ 1\n";
							cout << "введ≥ть:\n";
							for (auto& i : possible_ways)
							{
								cout << "<" << i << "> - щоб рухатис€ " << (i == 0 ? "вгору" : i == 1 ? "вниз" : i == 2 ? "вправо" : "вл≥во") << "\n";
							}
							cout << "-> ";
							cin >> choice;
							if (GameProccess.way_is_open(GameProccess.Player_1, Direction(choice)) && choice < 4)
							{
								GameProccess.Player_1.move(Direction(choice));
								break;
							}
						}
						
					}
					else if (user_game_choice == 2)
					{
						while (true)
						{
							int choice;
							char y_pos;
							int x_pos;
							system("cls");
							GameProccess.console_grid_output();
							cout << "\nчерга √равц€ 1\n";
							cout << "введ≥ть координату по горизонтал≥:\n";
							cin >> y_pos;
							y_pos -= 'A';
							cout << "введ≥ть координату по вертикал≥:\n";
							cin >> x_pos;
							cout << "введ≥ть:\n<0> - дл€ вертикального розположенн€\n<1> - дл€ горизонтального розположенн€\n";
							cin >> choice;
							if ((choice == 0 || choice == 1) && GameProccess.way_is_open(x_pos, int(y_pos), WallLocation(choice)))
							{
								GameProccess.locate_wall(GameProccess.Player_1, x_pos, int(y_pos), WallLocation(choice));
								break;
							}
						}
					}
				}
				else if (turn % 2 == 0)
				{
					while (true)
					{
						system("cls");
						GameProccess.console_grid_output();
						cout << "\nчерга √равц€ 2\n";
						if (GameProccess.Player_2.get_walls_number() > 0)
						{
							cout << "введ≥ть:\n<1> - дл€ перем≥щенн€\n<2> - дл€ розм≥щенн€ ст≥ни\n-> ";
							cin >> user_game_choice;
							if (user_game_choice == 1 || user_game_choice == 2)
								break;
						}
						else
						{
							cout << "введ≥ть:\n<1> - дл€ перем≥щенн€\n-> ";
							cin >> user_game_choice;
							if (user_game_choice == 1)
								break;
						}
					}
					if (user_game_choice == 1)
					{
						vector<int> possible_ways;

						if (GameProccess.way_is_open(GameProccess.Player_2, Direction::up))
							possible_ways.push_back(0);

						if (GameProccess.way_is_open(GameProccess.Player_2, Direction::down))
							possible_ways.push_back(1);

						if (GameProccess.way_is_open(GameProccess.Player_2, Direction::right))
							possible_ways.push_back(2);

						if (GameProccess.way_is_open(GameProccess.Player_2, Direction::left))
							possible_ways.push_back(3);

						while (true)
						{
							int choice;
							system("cls");
							GameProccess.console_grid_output();
							cout << "\nчерга √равц€ 2\n";
							cout << "введ≥ть:\n";
							for (auto& i : possible_ways)
							{
								cout << "<" << i << "> - щоб рухатис€ " << (i == 0 ? "вгору" : i == 1 ? "вниз" : i == 2 ? "вправо" : "вл≥во") << "\n";
							}
							cout << "-> ";
							cin >> choice;
							if (GameProccess.way_is_open(GameProccess.Player_2, Direction(choice)) && choice < 4)
							{
								GameProccess.Player_2.move(Direction(choice));
								break;
							}
						}

					}
					else if (user_game_choice == 2)
					{
						while (true)
						{
							int choice;
							char y_pos;
							int x_pos;
							system("cls");
							GameProccess.console_grid_output();
							cout << "\nчерга √равц€ 2\n";
							cout << "введ≥ть координату по горизонтал≥:\n";
							cin >> y_pos;
							y_pos -= 'A';
							cout << "введ≥ть координату по вертикал≥:\n";
							cin >> x_pos;
							cout << "введ≥ть:\n<0> - дл€ вертикального розположенн€\n<1> - дл€ горизонтального розположенн€\n";
							cin >> choice;
							if ((choice == 0 || choice == 1) && GameProccess.way_is_open(x_pos, int(y_pos), WallLocation(choice)))
							{
								GameProccess.locate_wall(GameProccess.Player_2, x_pos, int(y_pos), WallLocation(choice));
								break;
							}
						}
					}
				}
				turn++;
			}
			if (GameProccess.Player_1.get_pos_y() >= grid_size * 2 - 2)
			{
				system("cls");
				player_1_global_score++;
				cout << "√равець 1 - переможець!\nрахунок:\nгр1 - " << player_1_global_score << " : гр2 -" << player_2_global_score << "\n";
				system("pause");
				system("cls");
			}
			else
			{
				system("cls");
				player_2_global_score++;
				cout << "√равець 2 - переможець!\nрахунок:\nгр1 - " << player_1_global_score << " : гр2 -" << player_2_global_score << "\n";
				system("pause");
				system("cls");
			}

		}
	}
}
