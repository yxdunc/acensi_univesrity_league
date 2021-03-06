#include <iostream>
#include <vector>
#include <map>
using namespace std;

#define FIGX 15
#define FIGY 15

class coord
{
    public:
        coord() {};
        coord(int x, int y) : x(x), y(y) {};
        int x;
        int y;

		std::string serialize() { return (std::to_string(this->x) + " " + std::to_string(this->y)); };
};

std::vector<coord>  append(std::vector<coord> a, std::vector<coord> b)
{
    a.insert( a.end(), b.begin(), b.end() );
    return (a);
}
std::vector< coord >   find(std::vector< std::string > board, char c)
{
    std::vector< coord >    res;

    for (int x = 0; x < board.size(); x++)
    {
        for (int y = 0; y < board[0].size(); y++)
        {
            if(board[x][y] == c)
                res.push_back(coord(x, y));
        }
    }
	return(res);
}

bool    is_available(std::vector< std::string > board, coord pos)
{
    return (board[pos.x][pos.y] == '-') ? true : false;
}

std::vector< coord > is_neighbour(std::vector< std::string > board, coord pos, char c)
{
    std::vector< coord >    res;
    std::vector< coord >    n_pos;
    
    n_pos.push_back(coord(pos.x, pos.y + 1));
    n_pos.push_back(coord(pos.x, pos.y - 1));
    
    n_pos.push_back(coord(pos.x + 1, pos.y));
    n_pos.push_back(coord(pos.x + 1, pos.y + 1));
    n_pos.push_back(coord(pos.x + 1, pos.y - 1));
    
    n_pos.push_back(coord(pos.x - 1, pos.y));
    n_pos.push_back(coord(pos.x - 1, pos.y + 1));
    n_pos.push_back(coord(pos.x - 1, pos.y - 1));
    
    for (int i = 0; i < n_pos.size(); i++)
    {
        if(n_pos[i].x >= 0 && n_pos[i].y >= 0 && n_pos[i].x < 29 && n_pos[i].y < 29\
           && board[n_pos[i].x][n_pos[i].y] == c)
            res.push_back(n_pos[i]);
    }
    
    return (res);
}

std::vector< coord > neighbour_available(std::vector< std::string > board, coord pos)
{
    return (is_neighbour(board, pos, '-'));
}


bool    is_corner(std::vector< std::string > board, coord pos)
{
    if( (pos.x == 0 && pos.y == 0)
      ||(pos.x == 0 && pos.y == board.size() - 1)
      ||(pos.x == board.size() - 1 && pos.y == board[0].size() - 1)
      ||(pos.x == board.size() - 1 && pos.y == 0))
    {
        return (true);
    }
    return (false);
}

std::vector< coord > neighbour_unavailable(std::vector< std::string > board, coord pos, char player)
{
    std::vector < coord >   outsiders;
    
    if(is_corner(board, pos))
    {
        outsiders.push_back(coord(-1,-1));
        outsiders.push_back(coord(-1,-1));
        outsiders.push_back(coord(-1,-1));
        outsiders.push_back(coord(-1,-1));
        outsiders.push_back(coord(-1,-1));
        return (append(append(is_neighbour(board, pos, 'b'), is_neighbour(board, pos, 'w')), outsiders));
    }
    else
        return (append(is_neighbour(board, pos, 'b'), is_neighbour(board, pos, 'w')));
    
}

std::vector< coord >    neighbour_cells(std::vector< std::string > board)
{
    std::vector<coord>  res;
    for (int x = 0; x < board.size(); x++)
    {
        for (int y = 0; y < board[0].size(); y++)
        {
            if(neighbour_unavailable(board, coord(x, y), 'r').size() != 0)
                res.push_back(coord(x, y));
        }
    }
    return(res);
}
std::vector< coord >    check_exploder(std::vector<std::string> board, char player)
{
    std::vector< coord >    res;
    bool                    complet;
    
    complet = true;
    /* tiger */
/*    res.push_back(coord(15, 15));
    res.push_back(coord(15, 16));
    res.push_back(coord(13, 16));
    res.push_back(coord(14, 18));
    res.push_back(coord(15, 19));
    res.push_back(coord(15, 20));
    res.push_back(coord(15, 21));
*/    /**/
    /* heart patern */
    res.push_back(coord(FIGX, FIGY));
    res.push_back(coord(FIGX, FIGY + 2));
    res.push_back(coord(FIGX + 1, FIGY));
    res.push_back(coord(FIGX + 1, FIGY + 1));
    res.push_back(coord(FIGX + 1, FIGY + 2));
    res.push_back(coord(FIGX + 2, FIGY + 1));
    
    for (int i = 0; i < res.size(); i++)
    {
        if(is_neighbour(board, res[i], (player == 'w') ? 'b' : 'w').size() > 0)
            return (std::vector< coord >());
        if(board[res[i].x][res[i].y] == '-')
            complet = false;
    }
    if (complet)
        return (std::vector< coord >());
    return (res);
}

bool    print_first_av(std::vector<std::string> board, std::vector< coord > explo)
{
    if(explo.size() == 0)
        return (false);
    for (int i = 0; i < explo.size(); i++)
    {    
        if (is_available(board, explo[i]))
        {
            std::cout << explo[i].x << " " <<  explo[i].y << std::endl;
            return (true);
        }
    }
    return (false);
}
void    incr_score_neighbour(std::vector< coord > free_nb, std::map< std::string, int > &result, int inc)
{
    for(int i = 0; i < free_nb.size(); i++)
    {
        result[free_nb[i].serialize()] = result[free_nb[i].serialize()] + inc;
    }
}


void    suicide_penalty(std::vector<std::string> board, std::vector< coord > free_nb, std::map< std::string, int > &result, int inc, char player)
{
    unsigned int    unav;
    
    for(int i = 0; i < free_nb.size(); i++)
    {
        unav = neighbour_unavailable(board, free_nb[i], player).size();
        if(unav != 3 && unav != 2)
            result[free_nb[i].serialize()] = result[free_nb[i].serialize()] + inc;
    }
}

void    birthplace_bonus(std::vector<std::string> board, std::vector< coord > free_nb, std::map< std::string, int > &result, int inc, char player)
{
    std::vector <coord> nb;
    for(int i = 0; i < free_nb.size(); i++)
    {
        if(is_neighbour(board, free_nb[i], player).size() != 1 && neighbour_unavailable(board, free_nb[i], player).size() == 3)
        {
            std::cerr << "birth place at:" << free_nb[i].serialize() << std::endl;
            result[free_nb[i].serialize()] = result[free_nb[i].serialize()] + inc;
        }
        else if(is_neighbour(board, free_nb[i], player).size() == 2 && neighbour_unavailable(board, free_nb[i], player).size() == 2)
        {
            nb = neighbour_available(board, free_nb[i]);
            incr_score_neighbour(nb, result, 1); // ?
        }
        else if(is_neighbour(board, free_nb[i], player).size() == 3 && neighbour_unavailable(board, free_nb[i], player).size() == 3)
        {
            nb = neighbour_available(board, free_nb[i]);
            incr_score_neighbour(nb, result, -inc); // ?
        }
    }
}

bool    print_deadly(std::vector< std::string > board, std::vector< coord > them, std::vector< coord > us, char player, std::vector< coord > available)
{
    std::map< std::string, int >    result;
    std::pair< std::string, int >   max;
    std::vector<coord>              free_nb;
    std::vector<coord>              not_free_nb;

    max.second = -142;
    
    
    for (int i = 0; i < them.size(); i++)
    {
        free_nb = neighbour_available(board, them[i]);
        not_free_nb = neighbour_unavailable(board, them[i], player);
        if (not_free_nb.size() < 4)
            incr_score_neighbour(free_nb, result, 100);
    }
    for (int i = 0; i < us.size(); i++)
    {
        free_nb = neighbour_available(board, us[i]);
        not_free_nb = neighbour_unavailable(board, us[i], player);
        if (not_free_nb.size() == 1)
            incr_score_neighbour(free_nb, result, 99);
        else if (not_free_nb.size() < 4)
            incr_score_neighbour(free_nb, result, -1);
    }
    suicide_penalty(board, available, result, -100, player);
    birthplace_bonus(board, available, result, 99, player);
    
    typedef std::map< std::string, int >::iterator it_type;
    for(it_type it = result.begin(); it != result.end(); it++)
    {
        //std::cerr << "all.coord: " << it->first << std::endl;
        //std::cerr << "all.score: " << it->second << std::endl;
        if (it->second > max.second)
		{
            max.first = it->first;
            max.second = it->second;
		}
    }
    
    if(max.second != -142)
    {
        std::cout << max.first << std::endl;
        std::cerr << "result of max: " << max.second << std::endl;
        return(1);
    }
    else
        return(0);
}

void    sig_empty(const std::vector<std::string> board, std::vector<std::string> &new_board, coord pos, char player)
{
    std::vector<coord>  us;
    std::vector<coord>  them;
    char                them_c;
    
    them_c = (player == 'w') ? 'b' : 'w';
    us = is_neighbour(board, pos, player);
    them = is_neighbour(board, pos, them_c);
    if (us.size() + them.size() == 3)
        new_board[pos.x][pos.y] = (us.size() < them.size()) ? them_c : player;
}

void    sig_filled(const std::vector<std::string> board, std::vector<std::string> &new_board, coord pos)
{
    unsigned int    neighbours;
    
    neighbours = is_neighbour(board, pos, 'w').size() + is_neighbour(board, pos, 'b').size();
    if (neighbours != 3 && neighbours != 2)
        new_board[pos.x][pos.y] = '-';
}

std::vector<std::string>    simulate_step(std::vector<std::string> board, char player)
{
    coord   pos;
    std::vector<std::string>     new_board;
    
    new_board = board;
    for (pos.x = 0; pos.x < board.size(); pos.x++)
    {
        for (pos.y = 0; pos.y < board[0].size(); pos.y++)
        {
            if(board[pos.x][pos.y] == '-')
                sig_empty(board, new_board, pos, player);
            else if(board[pos.x][pos.y] == 'w' || board[pos.x][pos.y] == 'b')
                sig_filled(board, new_board, pos);
        }
    }
    return (new_board);
}

int     simulate_game(std::vector<std::string> board, char player, int depth)
{
    int     score;
    
    score = 0;
    
    for(int i = 0; i < depth; i++)
        board = simulate_step(board, player);
    
    score = find(board, player).size() - find(board, (player == 'w') ? 'b' : 'w').size();
    return (score);
}

bool    best_move(std::vector<std::string> board, std::vector<coord> moves, char player)
{
    std::vector<string> board_test;
    int     max;
    int     temp;
    coord   max_coord;
    
    max = -10;
    max_coord.x = moves[0].x;
    max_coord.y = moves[0].y;
    
    for (int i = 0; i < moves.size(); i++)
    {
        board_test = board;
        board_test[moves[i].x][moves[i].y] = player;
        temp = simulate_game(board, player, 1);
        std::cerr << "score: " << temp << std::endl;
        if(temp > max)
        {
            max = temp;
            max_coord = moves[i];
        }
    }
    std::cout << max_coord.x << " " << max_coord.y << std::endl;
    return (true); 
}

void nextMove(char player, vector <string> board)
{
    std::vector< coord >    us;
    std::vector< coord >    them;
    std::vector< coord >    available;
    std::vector< coord >    exploder;

//    us = find(board, player);
    them = find(board, (player == 'w') ? 'b' : 'w');
    available = find(board, '-');
//    exploder = check_exploder(board, player);

    if (them.size() == 0)
    {
        std::cout << FIGX << " " <<  FIGY << std::endl;
        return ;
    }
    std::cerr << neighbour_cells(board).size() << std::endl;
    if (best_move(board, neighbour_cells(board), player)) return ;
    if (print_deadly(board, them, us, player, available)) return ;
    if (print_first_av(board, exploder)) return ;
    std::cout << available[available.size()/5].x << " " <<  available[available.size()/5].y << std::endl;
    
    return ;
}

int main(void)
{
    char player;
    vector <string> board;

    cin >> player;

    for(int i=0; i<29; i++) {
        string s; cin >> s;
        board.push_back(s);
    }

    nextMove(player,board);

    return 0;
}
