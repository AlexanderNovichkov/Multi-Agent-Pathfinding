#ifndef GL_CONST_H
#define	GL_CONST_H

#define CN_PI_CONSTANT 3.14159265359
#define CN_SQRT_TWO    1.41421356237
#define MAX_TIME       1000000000

//XML tags
#define CNS_TAG_ROOT "root"

    #define CNS_TAG_MAP                 "map"
        #define CNS_TAG_CELLSIZE        "cellsize"
        #define CNS_TAG_WIDTH           "width"
        #define CNS_TAG_HEIGHT          "height"
        // #define CNS_TAG_STX         "startx"
        // #define CNS_TAG_STY         "starty"
        // #define CNS_TAG_FINX        "finishx"
        // #define CNS_TAG_FINY        "finishy"
        #define CNS_TAG_AGENTS          "agents"
            #define CNS_TAG_AGENTDATA   "agentdata"
        #define CNS_TAG_GRID            "grid"
            #define CNS_TAG_ROW         "row"

    #define CNS_TAG_ALG             "algorithm"
        // #define CNS_TAG_ST          "searchtype"
        #define CNS_TAG_HW          "hweight"
        #define CNS_TAG_MT          "metrictype"
        #define CNS_TAG_BT          "breakingties"
        // #define CNS_TAG_AS          "allowsqueeze"
        // #define CNS_TAG_AD          "allowdiagonal"
        // #define CNS_TAG_CC          "cutcorners"
        #define CNS_TAG_AP          "agentpriority"
        #define CNS_TAG_BS          "blockstartduration"
        #define CNS_TAG_TP          "tryotherpriority"
        #define CNS_TAG_TL          "runtimelimit"

    #define CNS_TAG_OPT             "options"
        #define CNS_TAG_LOGLVL      "loglevel"
        #define CNS_TAG_LOGPATH     "logpath"
        #define CNS_TAG_LOGFN       "logfilename"

    #define CNS_TAG_LOG             "log"
        #define CNS_TAG_MAPFN       "mapfilename"
        #define CNS_TAG_SUM         "summary"
        #define CNS_TAG_TRAJECTORY  "trajectory"
            #define CNS_TAG_ACTION  "action"
        #define CNS_TAG_AGENT       "agent"
        #define CNS_TAG_PATH        "path"
        #define CNS_TAG_LPLEVEL     "lplevel"
        #define CNS_TAG_HPLEVEL     "hplevel"
            #define CNS_TAG_SECTION "section"
        #define CNS_TAG_LOWLEVEL    "lowlevel"
            #define CNS_TAG_STEP    "step"
            #define CNS_TAG_OPEN    "open"
            #define CNS_TAG_POINT   "node"
            #define CNS_TAG_CLOSE   "close"

//XML tags' attributes
    #define CNS_TAG_ATTR_SOLUTION_FOUND     "solution_found"
    #define CNS_TAG_ATTR_COST               "cost"
    #define CNS_TAG_ATTR_TRAJECTORY_FOUND   "trajectory_found"
    #define CNS_TAG_ATTR_TRAJECTORY_TIME    "trajectory_execution_time"
    #define CNS_TAG_ATTR_NUMOFSTEPS         "numberofsteps"
    #define CNS_TAG_ATTR_NODESCREATED       "nodescreated"
    #define CNS_TAG_ATTR_LENGTH             "length"
    #define CNS_TAG_ATTR_LENGTH_SCALED      "length_scaled"
    #define CNS_TAG_ATTR_SEARCH_RUNTIME     "runtime"
    #define CNS_TAG_ATTR_START_TIME         "start_time"
    #define CNS_TAG_ATTR_X                  "x"
    #define CNS_TAG_ATTR_Y                  "y"
    #define CNS_TAG_ATTR_NUM                "number"
    #define CNS_TAG_ATTR_F                  "F"
    #define CNS_TAG_ATTR_G                  "g"
    #define CNS_TAG_ATTR_PARX               "parent_x"
    #define CNS_TAG_ATTR_PARY               "parent_y"
    #define CNS_TAG_ATTR_STX                "start.x"
    #define CNS_TAG_ATTR_STY                "start.y"
    #define CNS_TAG_ATTR_FINX               "finish.x"
    #define CNS_TAG_ATTR_FINY               "finish.y"


//Search Parameters
    #define CN_SP_ST 0

        // #define CNS_SP_ST_BFS           "bfs"
        // #define CNS_SP_ST_DIJK          "dijkstra"
        // #define CNS_SP_ST_ASTAR         "astar"
        // #define CNS_SP_ST_JP_SEARCH     "jp_search"
        // #define CNS_SP_ST_TH            "theta"
        #define CNS_SP_ST_MAPF            "mapf"
        // #define CN_SP_ST_BFS            0
        // #define CN_SP_ST_DIJK           1
        // #define CN_SP_ST_ASTAR          2
        // #define CN_SP_ST_JP_SEARCH      3
        // #define CN_SP_ST_TH             4
        #define CN_SP_ST_MAPF              5

    // #define CN_SP_AD 1 //AllowDiagonal

    // #define CN_SP_CC 2 //CutCorners

    // #define CN_SP_AS 3 //AllowSqueeze

    #define CN_SP_MT 4 //MetricType

        #define CNS_SP_MT_DIAG  "diagonal"
        #define CNS_SP_MT_MANH  "manhattan"
        #define CNS_SP_MT_EUCL  "euclidean"
        #define CNS_SP_MT_CHEB  "chebyshev"
        #define CNS_SP_MT_ZERO  "zero"

        #define CN_SP_MT_DIAG   0
        #define CN_SP_MT_MANH   1
        #define CN_SP_MT_EUCL   2
        #define CN_SP_MT_CHEB   3
        #define CN_SP_MT_ZERO   4

    #define CN_SP_HW 5 //HeuristicWeight

    #define CN_SP_BT 6 //BreakingTies

        #define CNS_SP_BT_GMIN "g-min"
        #define CNS_SP_BT_GMAX "g-max"

        #define CN_SP_BT_GMIN 0
        #define CN_SP_BT_GMAX 1

    #define CN_SP_AP 7
        #define CNS_SP_AP_RAND  "random"
        #define CNS_SP_AP_HRMIN "heuristic-min"
        #define CNS_SP_AP_HRMAX "heuristic-max"
        #define CNS_SP_AP_FIFO  "fifo"

        #define CN_SP_AP_RAND   0
        #define CN_SP_AP_HRMIN  1
        #define CN_SP_AP_HRMAX  2
        #define CN_SP_AP_FIFO   3

    #define CN_SP_BS 8
    #define CN_SP_TP 9
    #define CN_SP_TL 10


    //Log Configuration
    #define CN_LP_LEVEL 0

        #define CN_LP_LEVEL_NOPE_VALUE      "0"
        #define CN_LP_LEVEL_NOPE_WORD       "none"
        #define CN_LP_LEVEL_TINY_VALUE      "0.5"
        #define CN_LP_LEVEL_TINY_WORD       "tiny"
        #define CN_LP_LEVEL_SHORT_VALUE     "1"
        #define CN_LP_LEVEL_SHORT_WORD      "short"
        #define CN_LP_LEVEL_MEDIUM_VALUE    "1.5"
        #define CN_LP_LEVEL_MEDIUM_WORD     "medium"
        #define CN_LP_LEVEL_FULL_VALUE      "2"
        #define CN_LP_LEVEL_FULL_WORD       "full"

    #define CN_LP_PATH 1
    #define CN_LP_NAME 2


//Grid Cell
    #define CN_GC_NOOBS 0
    #define CN_GC_OBS   1

//Other
    #define CNS_OTHER_PATHSELECTION     "*"
    #define CNS_OTHER_MATRIXSEPARATOR   ' '

#endif

