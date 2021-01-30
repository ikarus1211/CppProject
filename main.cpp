#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#define BOARD_SIZE 8
class Board{
public:
    // Main board layou
    std::vector<std::vector<char>> boardContent = {{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                                   {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                                   {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                                   {' ', ' ', ' ', '@', '#', ' ', ' ', ' '},
                                                   {' ', ' ', ' ', '#', '@', ' ', ' ', ' '},
                                                   {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                                   {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                                   {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

    Board(){}


    // Gets a 2D vector as parameter which is then formatted and printed into console
    void printBoard(std::vector<std::vector<char>> board){
        std::cout << "--------------------------------------------------------------\n";
        std::cout << "|                            OTHELLO                         |\n";
        std::cout << "--------------------------------------------------------------\n";
        std::cout << "\n\n";
        std::cout << "------------A-------B-------C-------D-------E-------F-------G-------H---------\n";
        std::string line1 = " \xc4\xc4\xc4\xc4\xc4\xc4\xc4\xce\xc4\xc4\xc4";
        std::string line2 = "\xc4\xc4\xc4\xc4\xc5\xc4\xc4\xc4";
        std::string line3 = "\xc4\xc4\xc4\xc4\xce\xc4\xc4\xc4\xc4\xc4\xc4\xc4";
        std::string lineS = "\xcd\xcd\xcd\xcd\xcb\xcd\xcd\xcd";
        std::string lineE = "\xcd\xcd\xcd\xcd\xca\xcd\xcd\xcd";

        std::cout << "\xcd\xcd\xcd\xcd" << lineS;
        for (int k = 0; k < 7; ++k) {
            std::cout << lineS;
        }
        std::cout << lineS << "\xcd\xcd\xcd\xcd" << std::endl;

        for (int i = 0; i < 8; i++){
            std::cout << "   "<< i + 1<< " ";
            for (int j = 0; j < 8; ++j) {
                if(j % 8 == 0)
                    std::cout << "   \xba   " << board[i][j];
                else
                    std::cout << "   \xb3   " << board[i][j];
            }
            std::cout << "   \xba   " << i + 1 << std::endl;
            if (i == 7)
                continue;
            std::cout << line1;
            for (int k = 0; k < 7; ++k) {
                std::cout << line2;
            }
            std::cout << line3 << std::endl;
        }

        std::cout << "\xcd\xcd\xcd\xcd" << lineE;
        for (int k = 0; k < 7; ++k) {
            std::cout << lineE;
        }
        std::cout << lineE << "\xcd\xcd\xcd\xcd" << std::endl;

        std::cout << "------------A-------B-------C-------D-------E-------F-------G-------H---------\n";
    }

    // Function which flips playing stones in line
    std::vector<std::vector<char>> _flipLeftRight(int line, int startCol, int endCol, char p,
                                                  std::vector<std::vector<char>> b){
        // Sort index from smaller to higher
        if(startCol > endCol){
            int tmp = startCol;
            startCol = endCol;
            endCol = tmp;
        }
        for (int i = startCol; i <= endCol; ++i) {
            b[line][i] = p;
        }
        return b;
    }

    // Function which flips playing stones in column
    std::vector<std::vector<char>> _flipUpDown(int col, int startLine, int endLine, char p,
                                               std::vector<std::vector<char>> b){
        // Sort index from smaller to higher
        if(startLine > endLine){
            int tmp = startLine;
            startLine = endLine;
            endLine = tmp;
        }
        for (int i = startLine; i <= endLine; ++i) {
            b[i][col] = p;
        }
        return b;
    }

    // Function which flips playing stones in first diagonal
    std::vector<std::vector<char>> _flipFirstDiagonal(int startLine, int startCol, int endLine, int endCol, char p,
                                                      std::vector<std::vector<char>> b){
        // Sort index from smaller to higher
        if(startLine > endLine){
            int tmp = startLine;
            startLine = endLine;
            endLine = tmp;
            startCol = endCol;
        }

        for (int i = 0; i <= endLine - startLine; ++i) {
            b[startLine + i][startCol + i] = p;
        }
        return b;
    }

    // Function which flips playing stones in second diagonal
    std::vector<std::vector<char>> _flipSecondDiagonal(int startLine, int startCol, int endLine, int endCol, char p,
                                                       std::vector<std::vector<char>> b){
        // Sort index from smaller to higher
        if(startLine > endLine){
            int tmp = startLine;
            startLine = endLine;
            endLine = tmp;
            startCol = endCol;
        }

        for (int i = 0; i <= endLine - startLine; ++i) {
            b[startLine + i][startCol - i] = p;
        }
        return b;
    }

};


class Game{

public:
    typedef struct MoveTypes{
        int moveType;
        int startL;
        int startC;
        int finishL;
        int finishC;
    } MType;

    Game(char p, char ai, char scoreM, int maxD, double maxT){
        HumaPlayer = p;
        if (p == '@')
            AIPlayer = '#';
        else
            AIPlayer = '@';
        AIMode = ai;
        scoreMode = scoreM;
        maxDepth = maxD;
        maxTime = std::chrono::duration<double>(maxT);
    }

    void startGame(){
        _runMainLoop();
    }


private:

    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> maxTime;
    std::vector<int> bestSequenceMoves;
    int bestSequenceDepth = INT_MIN;
    std::chrono::time_point<std::chrono::system_clock> bestSequenceTimeCounter;
    int bestSequenceScore;
    char scoreMode;
    int maxDepth;
    int board_size = 8;
    char HumaPlayer = '#';
    char AIMode;
    Board board;
    char AIPlayer = '@';

    /*
     * Main loop of game.
     */

    int _runMainLoop() {

        board.printBoard(board.boardContent);
        while (true) {
            std::string inputString;
            bool noMoveForPlayer, noMoveForAi;

            if ( _findMoves(HumaPlayer,AIPlayer,board.boardContent).empty()){
                noMoveForPlayer = true;
                if (!_waitForSkip())
                    break;
            } else {
                noMoveForPlayer = false;
                if(!_waitForMove())
                    break;
            }

            bool AIMoveRes = false;
            if (AIMode == 'r'){
                AIMoveRes = _randomAiMove(AIPlayer, HumaPlayer);
            } else if (AIMode == 'a') {
                start = std::chrono::system_clock::now();
                bestSequenceTimeCounter = start;
                AIMoveRes = _alphaBetaPruning(AIPlayer);
            }

            if (!AIMoveRes){
                noMoveForAi = true;
                std::cout << "No valid move for AI. Press key to continue\n";
                std::cin.get();
            } else {
                noMoveForAi = false;
                board.printBoard(board.boardContent);
            }
            if (noMoveForAi && noMoveForPlayer){
                _checkWinner();
                std::cout << "Game finished\n";
                return 4;
            }
        }
        return 0;
    }
    // Function is called after all players are out of moves.
    void _checkWinner(){
        int hashtagPoints = 0;
        int atPoints = 0;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board.boardContent[i][j] == '@')
                    atPoints++;
                else if (board.boardContent[i][j] == '#')
                    hashtagPoints++;
            }
        }

        if (hashtagPoints > atPoints)
            std::cout << "WINNER IS ; # ; with " << hashtagPoints << " points versus " << atPoints << "\n";
        if (atPoints > hashtagPoints)
            std::cout << "WINNER IS ; @ ; with " << atPoints << " points versus " << hashtagPoints << "\n";
        if (atPoints == hashtagPoints)
            std::cout << "LOL IT'S A DRAW\n";
    }

    // Waiting for player to skip his turn because he has no playable moves
    bool _waitForSkip(){
        while(true){
            std::string inputString;
            std::cout << "No valid moves to play. Please type 's' for skip or 'q' for quit\n";
            std::cin >> inputString;
            if (inputString[0] == 'q')
                return false;
            if (inputString[0] == 's')
                return true;
        }
    }
    // If player has a valid moves, this function waits until one of them is played
    bool _waitForMove() {
        std::string inputString;
        while (true) {
            std::cout << "Command examples: place token ('a1'), quit ('q') \n";
            std::cout << "Write a move:";
            std::cin >> inputString;
            std::cout << inputString << std::endl;
            if (inputString[0] == 'q')
                return false;
            else {
                // When the move is played check if its valid
                if (_evaluateMove(inputString, HumaPlayer, AIPlayer)) {
                    board.printBoard(board.boardContent);
                    return true;
                } else {
                    board.printBoard(board.boardContent);
                    std::cout << "*** Invalid move, please try again ***\n";
                }
            }
        }
    }
    // This function checks the formatting of input and then sends it over to processing
    bool _evaluateMove(std::string moveString, char player, char opponent){

        if (moveString[0] >= 'a' && moveString[0] <= 'h' && moveString[1] >= '1' && moveString[1] <= '8') {
            int column;
            int line;
            column = moveString[0] - 'a';
            line = moveString[1] - '1';
            std::cout << column << " " << line << std::endl;

            // After format check send it on logical check
            if (board.boardContent[line][column] == ' ') {
                // If its valid if Yes flip stone if no return invalid move
                std::vector<MType> validMoves = _checkIfValidMove(line, column, player, opponent, board.boardContent);
                if (!validMoves.empty()) {
                    board.boardContent = _flipStones(validMoves, player, board.boardContent);
                    return true;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }
    }

    // Check which type of move has to be made and then flip stones.
    std::vector<std::vector<char>> _flipStones(std::vector<MType> validMoves, char player, std::vector<std::vector<char>> localBoard){


        for(MType m : validMoves){
            if(m.moveType == 0 || m.moveType == 4)
              localBoard = board._flipLeftRight(m.startL, m.startC, m.finishC, player, localBoard);
            if(m.moveType == 1 || m.moveType == 5)
                localBoard = board._flipFirstDiagonal(m.startL, m.startC, m.finishL, m.finishC, player, localBoard);
            if(m.moveType == 2 || m.moveType == 6)
                localBoard = board._flipUpDown(m.startC, m.startL, m.finishL, player, localBoard);
            if(m.moveType == 3 || m.moveType == 7)
                localBoard = board._flipSecondDiagonal(m.startL, m.startC, m.finishL, m.finishC, player, localBoard);
        }
        return localBoard;
    }

    // Find all playable moves on local board and returns all of them
    std::vector<std::vector<MType>> _findMoves(char p, char o, std::vector<std::vector<char>> localBoard){

        std::vector<std::vector<MType>> allValidMoves;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                std::vector<MType> m;
                if (localBoard[i][j] == ' ')
                    //We check for each move if it's valid
                    m = _checkIfValidMove(i,j,p,o, localBoard);
                if (!m.empty())
                    allValidMoves.emplace_back(m);
            }
        }
        return allValidMoves;
    }

    /*
     *  Function checks all direction for valid moves. If the move has at least one direction, where it can
     *  capture(flip) opponents stones, it is valid and added to return list.
     *  @Param  line - index of move we checking
     *          column - index of move we checking
     *          player - char representing player pieces which has move
     *          opponent - char representing opponent pieces
     *
     * Return all direction in which stones can be captured
     */
    std::vector<MType> _checkIfValidMove(int line, int column, char player, char opponent, std::vector<std::vector<char>> localBoard){

        bool dirFlags[8] = {0};
        std::vector<MType> validMove;
        //int startEndLocation[8][3];
        for (int i = 1; i < BOARD_SIZE; ++i) {

            // Left Direction
            if(!dirFlags[0] && column - i >= 0) {
                if (localBoard[line][column - i] != opponent){
                    if (localBoard[line][column - i] == player && i > 1)
                        validMove.emplace_back(MType{0, line, column, 0,column-i});
                    dirFlags[0] = true;
                }
            }

            // Left Up Direction
            if(!dirFlags[1] && line - i >= 0 && column - i >= 0) {
                if ( localBoard[line - i][column - i] != opponent){
                    if (localBoard[line - i][column - i] == player && i > 1)
                        validMove.emplace_back(MType{1, line, column, line - i, column - i});
                    dirFlags[1] = true;
                }

            }
            // Up Direction
            if(!dirFlags[2] && line - i >= 0 ) {
                if (localBoard[line - i][column] != opponent){
                    if (localBoard[line - i][column] == player && i > 1)
                        validMove.emplace_back(MType{2, line, column, line - i,0});
                    dirFlags[2] = true;
                }
            }
            // Up Right Direction
            if(!dirFlags[3] && line - i >= 0 && column + i < board_size) {
                if (localBoard[line - i][column + i] != opponent){
                    if (localBoard[line - i][column + i] == player && i > 1)
                        validMove.emplace_back(MType{3, line, column, line - i, column + i});
                    dirFlags[3] = true;
                }
            }
            // Right Direction
            if(!dirFlags[4] && column + i < board_size) {
                if (localBoard[line][column + i] != opponent){
                    if (localBoard[line][column + i] == player && i > 1)
                        validMove.emplace_back(MType{4, line, column, 0, column + i});
                    dirFlags[4] = true;
                }
            }
            // Right Down Direction
            if(!dirFlags[5] && line + i < board_size && column + i < board_size) {
                if (localBoard[line + i][column + i] != opponent){
                    if (localBoard[line + i][column + i] == player && i > 1)
                        validMove.emplace_back(MType{5, line, column, line + i, column + i});
                    dirFlags[5] = true;
                }
            }
            // Down Direction
            if(!dirFlags[6] && line + i < board_size ) {
                if (localBoard[line + i][column] != opponent) {
                    if (localBoard[line + i][column] == player && i > 1)
                        validMove.emplace_back(MType{6, line, column, line + i, 0});
                    dirFlags[6] = true;
                }
            }
            // Down Left Direction
            if(!dirFlags[7] && line + i < board_size && column - i >= 0 ) {
                if (localBoard[line + i][column - i] != opponent){
                    if (localBoard[line + i][column - i] == player && i > 1)
                        validMove.emplace_back(MType{7, line, column, line + i, column - i});
                    dirFlags[7] = true;
                }
            }
        }
        return validMove;
    }

    //AIs Section

    // Random AI. Selects move on random basis
    bool _randomAiMove(char p, char o){
        // Find all possible moves
        std::vector<std::vector<MType>> allValidMoves = _findMoves(p,o,board.boardContent);

        // Execute random move
        if(!allValidMoves.empty()){
            // source
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<double> dist(0, allValidMoves.size());
            int index = static_cast<int>(dist(mt));
            board.boardContent = _flipStones(allValidMoves[index],p , board.boardContent);
            return true;
        } else {
            return false;
        }
    }

    // Alpha beta pruning
    bool _alphaBetaPruning(char ai) {
        int result;
        int moveIndex = -1;
        std::vector<std::pair<int, int>> moveSequence;
        // If AI plays as "#" white
        if(ai == '#'){
            // Find best playable moves
            std::pair<std::vector<int>, int> bestMove = minmax(INT_MIN, INT_MAX, true , maxDepth,
                    board.boardContent, '#', '@',moveSequence);
            // No move was found  that means no move can be played
            if (bestMove.first[0] < 0)
                return false;

            else{
                std::vector<std::vector<MType>> allValidMoves = _findMoves('#','@', board.boardContent);
                std::cout << std::endl;
                printMoveSequence(bestMove.first, bestMove.second);
                int bestStartLine = bestMove.first[0] / 10 - 1;
                int bestStartColumn = bestMove.first[0] % 10 - 1;
                for (int i = 0; i < allValidMoves.size() ; ++i) {
                    if (allValidMoves[i][0].startL == bestStartLine && allValidMoves[i][0].startC == bestStartColumn) {
                        board.boardContent = _flipStones(allValidMoves[i], '#', board.boardContent);
                        break;
                    }
                }
                return true;
            }
        }
        // If AI plays as "@" black
        else {

            // Find best playable moves
            std::pair<std::vector<int>, int> bestMove = minmax(INT_MIN, INT_MAX,false , maxDepth,
                                                  board.boardContent, '@', '#',moveSequence );
            // No move was found  that means no move can be played
            if (bestMove.first[0] < 0)
                return false;
            else {
                std::cout << std::endl;
                printMoveSequence(bestMove.first, bestMove.second);
                std::vector<std::vector<MType>> allValidMoves = _findMoves('@','#',board.boardContent);
                int bestStartLine = bestMove.first[0] / 10 - 1;
                int bestStartColumn = bestMove.first[0] % 10 - 1;
                for (int i = 0; i < allValidMoves.size() ; ++i) {
                    if (allValidMoves[i][0].startL == bestStartLine && allValidMoves[i][0].startC == bestStartColumn) {
                        board.boardContent = _flipStones(allValidMoves[i], '@', board.boardContent);
                        break;
                    }
                }
                return true;
            }
        }
    }


    /*
     * Recursive minmax function with alpha beta pruning
     * @Param alpha - highest value found so far
     *        beta - lowest value found so far
     *        isMax - decide if min is on turn or max
     *        depth - max depth
     *        b - board that is passed through recursion
     *        you - color of stone on turn
     *        opp - color of opponent stone
     * Returns best score that was found
     */
    std::pair<std::vector<int>, int> minmax(int alpha, int beta, bool isMax, int depth, std::vector<std::vector<char>> b,
                char onTurnPlayer, char offTurnPlayer, std::vector<std::pair<int, int>> moveSequence){

        // Max depth has been reached
        if (depth <= 0) {
            // Count the score you found
            std::vector<int> ret;
            for (std::pair<int,int> p : moveSequence) {
                ret.emplace_back(p.first);
            }
            return std::make_pair(ret, _countScoreHeu(b));
        }

        // No valid moves to play
        std::vector<std::vector<MType>> allValidMoves = _findMoves(onTurnPlayer,offTurnPlayer, b);
        if (allValidMoves.empty()){
            std::vector<int> ret = {-11};
            return std::make_pair(ret,0);
        }

        if(isMax){
            int max = INT_MIN;
            int move = -1;
            std::vector<int> bestSoFar;
            for (int i = 0; i < allValidMoves.size() ; ++i) {

                // Checking max calculating time
                 end = std::chrono::system_clock::now();
                 std::chrono::duration<double> elapsedTime = end - start;
                 if (elapsedTime > maxTime)
                     break;
                // Periodic print out
                elapsedTime  = start - bestSequenceTimeCounter;
                if (elapsedTime.count() > 1.0){
                    printMoveSequence(bestSequenceMoves, bestSequenceScore);
                    bestSequenceTimeCounter = start;
                }
                 // Finding valid moves
                 std::vector<std::vector<char>> flipped(_flipStones(allValidMoves[i], onTurnPlayer, b));
                 int tmpMove = (allValidMoves[i][0].startL + 1) * 10 + (allValidMoves[i][0].startC + 1);
                 std::pair<int, int> tmpPair(tmpMove, max);
                 // Executing move
                 moveSequence.emplace_back(tmpPair);
                 std::pair<std::vector<int>, int> moveScore = minmax(alpha, beta, false, depth - 1, flipped, offTurnPlayer,
                                                        onTurnPlayer, moveSequence);

                if (moveScore.first[0] < 0) {
                    std::vector<int> ret;
                    for (std::pair<int, int> p : moveSequence) {
                        ret.emplace_back(p.first);
                    }
                }

                 moveSequence.pop_back();

                 // Comparing scores
                 if (max < moveScore.second) {
                     max = moveScore.second;
                     bestSoFar = moveScore.first;
                 }
                if (depth >= bestSequenceDepth){
                    bestSequenceDepth = depth;
                    bestSequenceMoves = bestSoFar;
                    bestSequenceScore = max;
                }
                 // Alpha beta pruning
                 alpha = std::max(max, alpha);

                 if (beta <= alpha){
                     break;
                 }

             }

             return std::make_pair(bestSoFar,max);
        } else {
             int min = INT_MAX;
             int move = -1;
             std::vector<int> bestSoFar;
             for (int i = 0; i < allValidMoves.size() ; ++i) {
                 std::vector<std::vector<char>> flipped(_flipStones(allValidMoves[i], onTurnPlayer, b));

                 end = std::chrono::system_clock::now();
                 std::chrono::duration<double> elapsedTime = end - start;
                 if (elapsedTime > maxTime)
                     break;

                 elapsedTime  = end - bestSequenceTimeCounter;
                 if (elapsedTime.count() > 1.0){
                     printMoveSequence(bestSequenceMoves, bestSequenceScore);
                     bestSequenceTimeCounter = end;
                 }
                 // Finding valid moves
                 int tmpMove = (allValidMoves[i][0].startL + 1) * 10 + (allValidMoves[i][0].startC + 1);
                 std::pair<int, int> tmpPair(tmpMove, min);
                 moveSequence.emplace_back(tmpPair);

                 // Executing move
                 std::pair<std::vector<int>, int> moveScore = minmax(alpha, beta, true, depth - 1, flipped, offTurnPlayer, onTurnPlayer, moveSequence);

                 // If no move was returned;
                 if (moveScore.first[0] < 0) {
                     std::vector<int> ret;
                     for (std::pair<int,int> p : moveSequence) {
                         ret.emplace_back(p.first);
                     }
                     moveScore = std::make_pair(ret, _countScoreHeu(flipped));
                 }

                 moveSequence.pop_back();

                 // Comparing scores
                 if (min > moveScore.second){
                     min = moveScore.second;
                     bestSoFar = moveScore.first;
                 }
                 if (depth >= bestSequenceDepth){
                     bestSequenceDepth = depth;
                     bestSequenceMoves = bestSoFar;
                     bestSequenceScore = min;
                 }
                 // Alpha beta pruning
                 beta = std::min(min, beta);
                 if (beta <= alpha){
                     break;
                 }

             }

             return std::make_pair(bestSoFar,min);
         }
     }
     /*
      *
      */
    int _countScoreHeu(std::vector<std::vector<char>> b){
        int counter = 0;
        // Material count
        for (int i = 0; i < board_size ; ++i) {
            for (int j = 0; j < board_size; ++j) {
                if(b[i][j] == '#')
                    counter++;
                if(b[i][j] == '@')
                    counter--;
            }
        }
        // Corners count
        for (int k = 0; k < 8; k += 7) {
            for (int i = 0; i < 8; i += 7) {
                if(b[k][i] == '#')
                    counter += 10;
                if(b[k][i] == '@')
                    counter -= 10;
            }

        }
        // Playable moves
        counter += (static_cast<int>(_findMoves('#','@', b).size()) * 2);
        counter -= (static_cast<int>(_findMoves('@', '#', b).size()) * 2);

        return counter;
    }
    int _countScoreMat(std::vector<std::vector<char>> b){
        int counter = 0;
        // Material count
        for (int i = 0; i < board_size ; ++i) {
            for (int j = 0; j < board_size; ++j) {
                if(b[i][j] == '#')
                    counter++;
                if(b[i][j] == '@')
                    counter--;
            }
        }
        return counter;
    }

    void printMoveSequence(std::vector<int> moveSequence, int score){
        for (int i = 0; i < moveSequence.size() ; ++i) {
            std::cout << moveSequence[i] << ", ";
        }
        std::cout << "score: " << score;
        std::cout << std::endl;
    }
};

int parseArguments(int argc, char** argv)
{
    char playerColor = '#';
    char AImode = 'a';
    char scoringMethod = 'h';
    int maxDepth = 6;
    double maxTime = 30;
    for (int i = 1; i < argc; i++) {
        bool correctionFlag = false;
        std::string inputArgument(argv[i]);
        if (inputArgument == "--black") {
            correctionFlag = true;
            playerColor = '@';
        } else if (inputArgument == "--white") {
            correctionFlag = true;
            playerColor = '#';
        } else if (inputArgument == "--func"){
            i++;
            std::string scoreType(argv[i]);
            if (scoreType == "h"){
                scoringMethod = 'h';
                correctionFlag = true;
            } else if (scoreType == "s") {
                scoringMethod = 's';
                correctionFlag = true;
            }
        } else if (inputArgument == "--depth") {
            i++;
            std::string maxd(argv[i]);
            maxDepth = std::stoi(maxd);
            correctionFlag = true;
        } else if (inputArgument == "--time") {
            i++;
            std::string maxt(argv[i]);
            maxTime = std::stoi(maxt);
            correctionFlag = true;
        }
        if (!correctionFlag){
            std::cout << "Wrong arguments\n";
            return 666;
        }

    }
    Game game(playerColor, AImode,scoringMethod, maxDepth, maxTime);
    game.startGame();
    return 0;
}

int main(int argc, char** argv){
    if (argc == 1){
        Game game('#','a','h', 8, 20);
        game.startGame();
    } else {
        return parseArguments(argc,argv);
    }
}