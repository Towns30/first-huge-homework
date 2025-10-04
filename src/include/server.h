#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <iostream>
int count_map[50][50] = {0};//此地图用于计数，辅助初始化地图，地雷在此地图里被赋值1，其余位置赋值0，这样方便对周围8个格子值求和
char map_printed[50][50];/*此地图为了PrintMap()函数而定义，此地图里存储的就是打印出来的地图，
未被访问就是'?'，对于已被访问的格子，'x'代表把非地雷标记为雷或者访问了地雷格，'@'代表标记地雷且正确，数字代表周围一圈有多少地雷*/
char real_map[50][50];//此地图代表真正的地图，其中'm'代表地雷，而非地雷格里的数字代表周围一圈有多少地雷
int column_back = -1, column_forward = 1, row_back = -1, row_forward = 1;/*back和forward代表着遍历行和列时需要回退和前进多少
也就是在[i + row_back, i + row_forward]与[j + column_back, j + column_forward]进行遍历*/
/*
 * You may need to define some global variables for the information of the game map here.
 * Although we don't encourage to use global variables in real cpp projects, you may have to use them because the use of
 * class is not taught yet. However, if you are member of A-class or have learnt the use of cpp class, member functions,
 * etc., you're free to modify this structure.
 */
int rows;         // The count of rows of the game map. You MUST NOT modify its name.
int columns;      // The count of columns of the game map. You MUST NOT modify its name.
int total_mines = 0;  // The count of mines of the game map. You MUST NOT modify its name. You should initialize this（需要初始化）
                  // variable in function InitMap. It will be used in the advanced task.
int game_state = 0;  // The state of the game, 0 for continuing, 1 for winning, -1 for losing. You MUST NOT modify its name.
int visit_count = 0;//玩家访问过的非地雷格子数
int marked_mine_count = 0;//玩家判定正确的地雷格子数
/**
 * @brief The definition of function InitMap()
 *
 * @details This function is designed to read the initial map from stdin. For example, if there is a 3 * 3 map in which
 * mines are located at (0, 1) and (1, 2) (0-based), the stdin would be
 *     3 3
 *     .X.
 *     ...
 *     ..X
 * where X stands for a mine block and . stands for a normal block. After executing this function, your game map
 * would be initialized, with all the blocks unvisited.
 */
void InitMap()//先生成count_map,再生成real_map和初始化map_printed
{
  for (int i = 0; i <= rows - 1; i++)
  {
    for (int j = 0; j <= columns - 1; j++)
    {
      real_map[i][j] = '0';
    }
  }
  char a;
  std::cin >> rows >> columns;
  for (int i = 0; i <= rows - 1; i++)
  {
    for (int j = 0; j <= columns - 1; j++)
    {
      std::cin >> a;
      if (a == 'X')
      {
        count_map[i][j] = 1;
        total_mines += 1;
      }
    }
  }
  for (int i = 0; i <= rows - 1; i++)
  {
    for (int j = 0; j <= columns - 1; j++)
    {
      if (count_map[i][j] == 1)
      {
        real_map[i][j] = 'm';
      }
      else 
      {
        row_back = (i = 0)? 0 : -1;
        row_forward = (i = rows - 1)? 0 : 1;
        column_back = (j = 0)? 0 : -1;
        column_forward = (j = columns - 1)? 0 : 1;
        for (int k = i + row_back; k <= i + row_forward; k++)
        {
          for (int m = j + column_back; m <= j + column_forward; m++)
          {
            if (k != i || m != j)//不包含此格子本身
            {
              real_map[i][j] = char(int(real_map[i][j] - 48) + count_map[k][m] + 48);//计算地雷的数量
            }
          }
        }
      }
    }
  }
  for (int i = 0; i <= rows - 1; i++)
  {
    for (int j = 0; j <= columns - 1; j++)
    {
      map_printed[i][j] = '?';//初始化map_printed
    }
  }
  // TODO (student): Implement me!
}

/**
 * @brief The definition of function VisitBlock(int, int)
 *
 * @details This function is designed to visit a block in the game map. We take the 3 * 3 game map above as an example.
 * At the beginning, if you call VisitBlock(0, 0), the return value would be 0 (game continues), and the game map would
 * be
 *     1??
 *     ???
 *     ???
 * If you call VisitBlock(0, 1) after that, the return value would be -1 (game ends and the players loses) , and the
 * game map would be
 *     1X?
 *     ???
 *     ???
 * If you call VisitBlock(0, 2), VisitBlock(2, 0), VisitBlock(1, 2) instead, the return value of the last operation
 * would be 1 (game ends and the player wins), and the game map would be
 *     1@1
 *     122
 *     01@
 *
 * @param r The row coordinate (0-based) of the block to be visited.
 * @param c The column coordinate (0-based) of the block to be visited.
 *
 * @note You should edit the value of game_state in this function. Precisely, edit it to
 *    0  if the game continues after visit that block, or that block has already been visited before.
 *    1  if the game ends and the player wins.
 *    -1 if the game ends and the player loses.
 *
 * @note For invalid operation, you should not do anything.
 */
void VisitBlock(int r, int c) 
{
  if (map_printed[r][c] == '?')//大前提：此格子未被访问
  {
    row_back = (r = 0)? 0 : -1;
    row_forward = (r = rows - 1)? 0 : 1;
    column_back = (c = 0)? 0 : -1;
    column_forward = (c = columns - 1)? 0 : 1;
    if (real_map[r][c] == 'm')//访问格为地雷
    {
      map_printed[r][c] = 'X';
      game_state = 1;
    }
    else if(real_map[r][c] >= '1' && real_map[r][c] <= '8')//访问格非雷但周围有地雷
    {
      map_printed[r][c] = real_map[r][c];
      visit_count += 1;
    }
    else if(real_map[r][c] == '0')//访问格非雷且周围无雷，需继续探索
    {
      map_printed[r][c] = '0';//把周围无雷的中心格显示出来
      visit_count += 1;//我们在编写程序的时候做到map_printed和visit_count一起改，这样使显示地图与计数同步
      for (int k = r + row_back; k <= r + row_forward; k++)
      {
        for (int m = c + column_back; m <= c + column_forward; m++)
        {
          if (k != r || m != c)//不包含此格子本身
          {
            VisitBlock(k, m);
          }
        }
      }
    }
  }
  // TODO (student): Implement me!
}

/**
 * @brief The definition of function MarkMine(int, int)
 *
 * @details This function is designed to mark a mine in the game map.
 * If the block being marked is a mine, show it as "@".
 * If the block being marked isn't a mine, END THE GAME immediately. (NOTE: This is not the same rule as the real
 * game) And you don't need to
 *
 * For example, if we use the same map as before, and the current state is:
 *     1?1
 *     ???
 *     ???
 * If you call MarkMine(0, 1), you marked the right mine. Then the resulting game map is:
 *     1@1
 *     ???
 *     ???
 * If you call MarkMine(1, 0), you marked the wrong mine(There's no mine in grid (1, 0)).
 * The game_state would be -1 and game ends immediately. The game map would be:
 *     1?1
 *     X??
 *     ???
 * This is different from the Minesweeper you've played. You should beware of that.
 *
 * @param r The row coordinate (0-based) of the block to be marked.
 * @param c The column coordinate (0-based) of the block to be marked.
 *
 * @note You should edit the value of game_state in this function. Precisely, edit it to
 *    0  if the game continues after visit that block, or that block has already been visited before.
 *    1  if the game ends and the player wins.
 *    -1 if the game ends and the player loses.
 *
 * @note For invalid operation, you should not do anything.
 */
void MarkMine(int r, int c) //把map_printed[r][c]标为雷
{
  if (map_printed[r][c] == '?')
  {
    if (real_map[r][c] == 'm' )
    {
      map_printed[r][c] = '@';
      marked_mine_count += 1;
    }
    else
    {
      map_printed[r][c] = 'X';
      game_state = 1;
    }
  }
  // TODO (student): Implement me!
}

/**
 * @brief The definition of function AutoExplore(int, int)
 *
 * @details This function is designed to auto-visit adjacent blocks of a certain block.
 * See README.md for more information
 *
 * For example, if we use the same map as before, and the current map is:
 *     ?@?
 *     ?2?
 *     ??@
 * Then auto explore is available only for block (1, 1). If you call AutoExplore(1, 1), the resulting map will be:
 *     1@1
 *     122
 *     01@
 * And the game ends (and player wins).
 */
void AutoExplore(int r, int c) //自动探索map_printed[r][c]
{
  int count_mines = 0;//记录格子周围一共有多少雷
  if (real_map[r][c] >= '1' && real_map[r][c] <= '8' && map_printed[r][c] == real_map[r][c])//已被访问的格且为数字格
  {
    row_back = (r = 0)? 0 : -1;
    row_forward = (r = rows - 1)? 0 : 1;
    column_back = (c = 0)? 0 : -1;
    column_forward = (c = columns - 1)? 0 : 1;
    for (int k = r + row_back; k <= r + row_forward; k++)
    {
      for (int m = c + column_back; m <= c + column_forward; m++)
      {
        if (map_printed[k][m] == '@')//计算已被标出的格子数
        {
          count_mines += 1;
        }
      }
    }
    if (count_mines == real_map[r][c])
    {
      for (int k = r + row_back; k <= r + row_forward; k++)
      {
        for (int m = c + column_back; m <= c + column_forward; m++)
        {
          if (map_printed[k][m] == '?')
          {
            VisitBlock(k, m);
          }
        }
      }
    }
  }
  // TODO (student): Implement me!
}

/**
 * @brief The definition of function ExitGame()
 *
 * @details This function is designed to exit the game.
 * It outputs a line according to the result, and a line of two integers, visit_count and marked_mine_count,
 * representing the number of blocks visited and the number of marked mines taken respectively.
 *
 * @note If the player wins, we consider that ALL mines are correctly marked.
 */
void ExitGame() 
{
  if (visit_count == (rows * columns - total_mines) || marked_mine_count == total_mines)
  {
    game_state = 1;
  }
  if (game_state == 1)
  {
    if (visit_count == (rows * columns - total_mines) || marked_mine_count == total_mines)
    {
      std::cout << "YOU WIN!";
    }
    else
    {
      std::cout << "GAME OVER!";
    }
    std::cout << visit_count << ' ' << marked_mine_count;
    exit(0);  // Exit the game immediately
  }
  // TODO (student): Implement me!
  
}

/**
 * @brief The definition of function PrintMap()
 *
 * @details This function is designed to print the game map to stdout. We take the 3 * 3 game map above as an example.
 * At the beginning, if you call PrintMap(), the stdout would be
 *    ???
 *    ???
 *    ???
 * If you call VisitBlock(2, 0) and PrintMap() after that, the stdout would be
 *    ???
 *    12?
 *    01?
 * If you call VisitBlock(0, 1) and PrintMap() after that, the stdout would be
 *    ?X?
 *    12?
 *    01?
 * If the player visits all blocks without mine and call PrintMap() after that, the stdout would be
 *    1@1
 *    122
 *    01@
 * (You may find the global variable game_state useful when implementing this function.)
 *
 * @note Use std::cout to print the game map, especially when you want to try the advanced task!!!
 */
void PrintMap() 
{
  /*for (int i = 0; i <= rows - 1; i++)
  {
    for (int j = 0; j <= columns - 1; j++)
    {
      std::cout << map_printed[i][j];
    }
    std::cout << std::endl;
  }*/
  for (int i = 0; i <= rows - 1; i++)
  {
    for (int j = 0; j <= columns - 1; j++)
    {
      std::cout << real_map[i][j];
    }
    std::cout << std::endl;
  }
  /*for (int i = 0; i <= rows - 1; i++)
  {
    for (int j = 0; j <= columns - 1; j++)
    {
      std::cout << count_map[i][j];
    }
    std::cout << std::endl;
  }*/
  // TODO (student): Implement me!
}
#endif
