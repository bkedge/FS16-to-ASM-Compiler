//Brady Kedge
//CS4280 - P1

#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "treePrint.h"


//Enum states just for clarification. s1 = 0  and so on
enum stateConstants {s1, s2, s3, s4, s5, s6, s7, s8, s9,
                    s10, s11, s12, s13, s14, s15, s16, s17,
                    s18, s19, s20, s21, s22, s23, s24, s25
                  };


int stateTable[25][24] =
{
             //  0    1   2   3   4   5   6    7   8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23
             //  L    D  EOF  WS  =   <   >    !   :    +    -    *    /    &    %    .    (    )    ,    {    }    ;    [    ]
  /* 0 - s1 */  {s2, s3, -1,  s1, s4, s8, s6, -2, s10, s11, s12, s13, s14, s15, s16, s17, s18, s19, s20, s21, s22, s23, s24, s25},
  /* 1 - s2 */  {s2, s2, 1000,1000,1000,1000,1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000},
  /* 2 - s3 */  {-2, s3, 1001,1001,1001,1001,1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001},
  /* 3 - s4 */  {1002, 1002, 1002,1002,1008,1002,1002, s5, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002},
  /* 4 - s5 */  {-2, -2, -2, -2,1005,-2,-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2},
  /* 5 - s6 */  {1004, 1004, 1004,1004,s7,1004,1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004},
  /* 6 - s7 */  {-2, -2, -2,-2,-2,-2,1006, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2},
  /* 7 - s8 */  {1003, 1003, 1003,1003,s9,1003,1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003},
  /* 8 - s9 */  {-2, -2, -2,-2,-2,1007,-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2},
  /* 9 - s10*/  {1009, 1009, 1009,1009,1009,1009,1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009},
  /* 10 - s11*/ {1010, 1010, 1010,1010,1010,1010,1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010},
  /* 11 - s12*/ {1011, 1011, 1011,1011,1011,1011,1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011},
  /* 12 - s13*/ {1012, 1012, 1012,1012,1012,1012,1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012},
  /* 13 - s14*/ {1013, 1013, 1013,1013,1013,1013,1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013},
  /* 14 - s15*/ {1014, 1014, 1014,1014,1014,1014,1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014},
  /* 15 - s16*/ {1015, 1015, 1015,1015,1015,1015,1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015},
  /* 16 - s17*/ {1016, 1016, 1016,1016,1016,1016,1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016},
  /* 17 - s18*/ {1017, 1017, 1017,1017,1017,1017,1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017},
  /* 18 - s19*/ {1018, 1018, 1018,1018,1018,1018,1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018},
  /* 19 - s20*/ {1019, 1019, 1019,1019,1019,1019,1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019},
  /* 20 - s21*/ {1020, 1020, 1020,1020,1020,1020,1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020},
  /* 21 - s22*/ {1021, 1021, 1021,1021,1021,1021,1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021},
  /* 22 - s23*/ {1022, 1022, 1022,1022,1022,1022,1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022},
  /* 23 - s24*/ {1023, 1023, 1023,1023,1023,1023,1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023},
  /* 24 - s25*/ {1024, 1024, 1024,1024,1024,1024,1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024}
};

//Pair states with tokens
tokenMap tokenStateMap[] =
{
  //Identifiers and numbers
  {1000, ID_Tk}, {1001, NUM_Tk}, {-1, EOF_Tk}, {-2, ERROR_Tk},

  //Relation operators
  {1002, EQUAL_Tk}, {1003, LessThan_Tk}, {1004, GreaterThan_Tk}, {1005, EqualBangEqual_Tk},
  {1006, GreaterThanEqual_Tk}, {1007, LessThanEqual_Tk}, {1008, DoubleEquals_Tk},

  //Other operators
  {1009, Colon_Tk}, {1010, Plus_Tk}, {1011, Minus_Tk}, {1012, Asterisk_Tk}, {1013, Slash_Tk}, {1014, Ampersand_Tk}, {1015, Percent_Tk},

  //Delimiters
  {1016, Period_Tk}, {1017, LeftParenth_Tk}, {1018, RightParenth_Tk}, {1019, Comma_Tk}, {1020, LeftBrace_Tk}, {1021, RightBrace_Tk},
  {1022, SemiColon_Tk}, {1023, LeftBracket_Tk}, {1024, RightBracket_Tk}

};

//Match keywords
token keywordTable[] =
{
  {BEGIN_Tk, "Begin", 0}, {END_Tk, "End", 0}, {START_Tk, "Start", 0},
  {STOP_Tk, "Stop", 0}, {IFF_Tk, "Iff", 0}, {LOOP_Tk, "Loop", 0},
  {VOID_Tk, "Void", 0}, {VAR_Tk, "Var", 0}, {INT_Tk, "Int", 0},
  {CALL_Tk, "Call", 0}, {RETURN_Tk, "Return", 0}, {SCAN_Tk, "Scan", 0},
  {PRINT_Tk, "Print", 0}, {PROGRAM_Tk, "Program", 0}
};

//To find character's column on the table
//  0   1  2  3   4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23
//  L   D EOF WS  =  <  >  !  :  +  -  *  /  &  %  .  (  )  ,  {  }  ;  [  ]
alphabetMap characterMapper[] =
{
  {'=',4},
  {'<',5},
  {'>',6},
  {'!',7},
  {':',8},
  {'+',9},
  {'-',10},
  {'*',11},
  {'/',12},
  {'&',13},
  {'%',14},
  {'.',15},
  {'(',16},
  {')',17},
  {',',18},
  {'{',19},
  {'}',20},
  {';',21},
  {'[',22},
  {']',23},
};

//Match the character parsed to the column in state table. Returns -1 if invalid character
int columnMatch(char lookahead)
{
  for(int i = 0; i < 20; i++)
  {
    if(characterMapper[i].key == lookahead)
    {
      return characterMapper[i].value;
    }
  }

  if(isalpha(lookahead))
  {
    return 0;
  }
  else if(isdigit(lookahead))
  {
    return 1;
  }
  else if(lookahead == EOF)
  {
    return 2;
  }
  else if(isspace(lookahead))
  {
    return 3;
  }
  else
  {
    return -1;
  }
}

//Matches token to word
token tokenMatch(int value, string word)
{
  token Tkn;

  for(int i = 0; i < 27; i++)
  {
    //Keyword match
    if(value == 1000)
    {
      for(int j = 0; j < 14; j++)
      {
        if(keywordTable[j].instance == word)
        {
          Tkn.tokenID = keywordTable[j].tokenID;
          Tkn.instance = word;
          Tkn.lineNum = 1;
        }
      }
    }

    if(tokenStateMap[i].key == value)
    {
      Tkn.tokenID = tokenStateMap[i].name;
      Tkn.instance = word;
      Tkn.lineNum = 1;
    }
  }

  return Tkn;

}

//Scanner
token scan(FILE *fp, int &linenumber)
{
  int lookahead; //Character scanned
  int currentState = 0;
  int nextState = 0;
  int column;

  string word;

  token Tkn;

  while(currentState < 1000 && currentState >= 0)
  {
    lookahead = fgetc(fp);

    //Filter comments until newline or whitespace
    if(lookahead == '@')
    {
      cout << "Comment" << endl;

      while(lookahead != '\n' && !isspace(lookahead))
      {
        lookahead = fgetc(fp);

        if(lookahead == '\n')
        {
          linenumber++;
        }
      }
    }

    //If invalid character
    if((column = columnMatch(lookahead)) == -1)
    {
      Tkn.tokenID = ERROR_Tk;
      Tkn.lineNum = linenumber;
      word += static_cast<char>(lookahead);
      cout << "Scanner encountered invalid character\n";
      Tkn.instance = word;
      return Tkn;
    }


    if(column == 3)
    {
      if(lookahead == '\n')
      {
        linenumber++;
      }

    }


    nextState = stateTable[currentState][column];

    if(nextState >= 1000 || nextState == -1 || nextState == -2)
    {
      if(lookahead == '\n')
      {
        linenumber++;
      }

      //EOF
      if(nextState == -1)
      {
        ungetc(lookahead, fp);
        Tkn.tokenID = EOF_Tk;
        Tkn.lineNum = linenumber;
        Tkn.instance = "EOF";
        return Tkn;
      }
      else if(nextState == 1005) //Handle =!=
      {
        ungetc(lookahead, fp);
        int lookaheadMore = fgetc(fp);
        word += static_cast<char>(lookaheadMore);
        Tkn = tokenMatch(nextState, word);
        Tkn.lineNum = linenumber;
        return Tkn;
      }
      else if(nextState == 1006) //Handle >=>
      {
        ungetc(lookahead, fp);
        int lookaheadMore = fgetc(fp);
        word += static_cast<char>(lookaheadMore);
        Tkn = tokenMatch(nextState, word);
        Tkn.lineNum = linenumber;
        return Tkn;
      }
      else if(nextState == 1007) //Handle <=<
      {
        ungetc(lookahead, fp);
        int lookaheadMore = fgetc(fp);
        word += static_cast<char>(lookaheadMore);
        Tkn = tokenMatch(nextState, word);
        Tkn.lineNum = linenumber;
        return Tkn;
      }
      else if(nextState == 1008) //Handle ==
      {
        ungetc(lookahead, fp);
        int lookaheadMore = fgetc(fp);
        word += static_cast<char>(lookaheadMore);
        Tkn = tokenMatch(nextState, word);
        Tkn.lineNum = linenumber;
        return Tkn;
      }
      else if(nextState >= 1000)//Final state
      {
        Tkn = tokenMatch(nextState, word);
        Tkn.lineNum = linenumber;
        ungetc(lookahead, fp);
        return Tkn;
      }
      else //Error
      {
        cout << "ERROR: Line: " << linenumber << " Instance: " << static_cast<char>(lookahead) << "\n";
        Tkn.tokenID = ERROR_Tk;
        Tkn.lineNum = linenumber;
        Tkn.instance = "Error";
        return Tkn;
      }

    }
    else
    {
      //Add character to word
      char nextVal = static_cast<char>(lookahead);

      if(!isspace(nextVal))
      {
        word += nextVal;
      }


      //Set current state to nextState
      currentState = nextState;
    }

  }

  //Failsafe
  Tkn.tokenID = ERROR_Tk;
  Tkn.lineNum = linenumber;
  Tkn.instance = "Scanner failed";
  return Tkn;
}
