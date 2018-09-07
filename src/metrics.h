// Copyright (c) 2016 The Zcash developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "uint256.h"

#include <atomic>
#include <mutex>
#include <string>

struct AtomicCounter {
    std::atomic<uint64_t> value;

    AtomicCounter() : value {0} { }

    void increment(){
        ++value;
    }

    void decrement(){
        --value;
    }

    int get() const {
        return value.load();
    }
};

class AtomicTimer {
private:
    std::mutex mtx;
    uint64_t threads;
    int64_t start_time;
    int64_t total_time;

public:
    AtomicTimer() : threads(0), start_time(0), total_time(0) {}

    /**
     * Starts timing on first call, and counts the number of calls.
     */
    void start();

    /**
     * Counts number of calls, and stops timing after it has been called as
     * many times as start().
     */
    void stop();

    bool running();

    uint64_t threadCount();

    double rate(const AtomicCounter& count);
};

extern AtomicCounter transactionsValidated;
extern AtomicCounter ehSolverRuns;
extern AtomicCounter solutionTargetChecks;
extern AtomicTimer miningTimer;

void TrackMinedBlock(uint256 hash);

void MarkStartTime();
double GetLocalSolPS();
int EstimateNetHeightInner(int height, int64_t tipmediantime,
                           int heightLastCheckpoint, int64_t timeLastCheckpoint,
                           int64_t genesisTime, int64_t targetSpacing);

void TriggerRefresh();

void ConnectMetricsScreen();
void ThreadShowMetricsScreen();

/**
 * bitcoin image: https://upload.wikimedia.org/wikipedia/commons/thumb/4/46/Bitcoin.svg/200px-Bitcoin.svg.png
 * License: CC BY-SA 3.0
 *
 * Rendering options:
 *
 */
const std::string METRICS_ART =
"[0;34;40m         [0;31;40m.[0;32;40m   [0;34;40m       [0;31;40m .[0;32;40m    [0;34;40m                                       [0;32;40m.[0;34;40m    [0;32;40m      [0;34;40m [0;32;40m           [0;34;40m  [0m\n"
"[0;34;40m       [0;31;40m [0;34;40mt[0;33;5;40;100m8@[0;1;30;90;43m88[0;37;43m@[0;1;33;93;43m.[0;1;33;93;47m8[0;1;31;91;43m8[0;33;47m8[0;1;30;90;43m8[0;33;5;40;100m%[0;31;5;40;100mS[0;32;40m;[0;34;40m.[0;31;40m.[0;32;40m    [0;34;40m          .[0;32;40mt[0;31;40m%[0;34;40mt[0;32;40m:[0;31;40m.[0;34;40m.[0;32;40m.[0;34;40m.[0;32;40m:[0;31;40m;[0;1;30;90;40mX[0;34;40m:[0;32;40m.[0;31;40m     [0;34;40m       [0;32;40m.[0;1;30;90;40mX[0;32;5;40;100m@[0;36;5;40;100m;[0;33;5;40;100mS[0;36;5;40;100m@[0;30;42m8[0;1;30;90;40mX[0;31;40m.[0;34;40m [0;32;40m  [0;34;40m.[0;32;40mS[0;32;5;40;100mX[0;33;5;40;100m;[0;36;5;40;100m%[0;33;5;40;100mS[0;32;5;40;100m8[0;32;40mt[0;34;40m;[0;31;40m.[0;32;40m    [0m\n"
"[0;34;40m     ;[0;33;5;40;100mS[0;37;43m@[0;33;5;43;103m [0;33;5;41;101m;[0;33;5;43;103m.[0;1;33;93;5;41;101m8[0;1;31;91;5;43;103m8[0;1;37;97;47m.[0;37;5;47;107m8[0;1;31;91;5;43;103m88[0;37;5;43;103m88[0;1;33;93;5;41;101m8[0;33;5;43;103m [0;37;5;41;101m8[0;33;47m8[0;33;5;40;100m8[0;1;30;90;40m@[0;31;40m.[0;34;40m         .[0;32;40m;[0;1;30;90;40m@X[0;34;40mt[0;32;40m;[0;31;40m:[0;34;40m;[0;32;40mt[0;1;30;90;40mX[0;34;40m;[0;31;40m;[0;34;40m;[0;32;40m;[0;34;40m;[0;32;40m%[0;1;30;90;40m8[0;31;40mt[0;34;40m;[0;32;40m.[0;31;40m    [0;34;40m [0;32;40m [0;34;40m.[0;32;40mX[0;1;30;90;42m8[0;33;5;42;102m@[0;1;37;97;47m   [0;1;36;96;47mS[0;1;33;93;47mX[0;1;37;97;47m [0;1;32;92;47m@[0;32;5;42;102m@[0;32;5;40;100m8[0;34;40m;[0;1;30;90;42m8[0;33;5;40;100mt[0;1;36;96;47m%[0;1;33;93;47mS[0;1;36;96;47mX[0;1;33;93;47mX[0;1;36;96;47mX[0;1;37;97;47m [0;1;32;92;47mX[0;33;5;42;102m%[0;32;5;40;100m8[0;32;40mS[0;34;40m:[0;31;40m.[0;32;40m [0m\n"
"[0;34;40m    [0;1;30;90;40m8[0;33;47m8[0;1;31;91;5;43;103m@[0;1;33;93;5;41;101m8[0;1;33;93;47m8[0;1;33;93;5;41;101m8[0;33;5;41;101m [0;37;5;43;103m8[0;1;31;91;5;43;103m8[0;37;5;47;107m8;[0;37;5;43;103m8[0;1;33;93;5;41;101m8[0;37;5;47;107m  [0;1;33;93;47m8[0;33;5;41;101m [0;37;5;43;103m%[0;1;33;93;5;41;101m8[0;33;5;43;103m [0;33;47mS[0;35;5;40;100m@[0;32;40m      [0;34;40m  [0;32;40m;[0;1;30;90;40m8[0;34;40m:[0;31;40m:[0;34;40m.[0;31;40m:[0;30;5;40;100m@[0;36;5;40;100m [0;37;5;40;100m8[0;1;30;90;47m8@@[0;37;5;40;100m8[0;33;5;40;100m [0;35;5;40;100m:[0;32;40m:[0;31;40m:[0;34;40m;[0;32;40m:[0;1;30;90;40mSS[0;31;40m    [0;32;40m  [0;30;5;40;100m8[0;1;32;92;42m%[0;32;5;42;102mX[0;1;36;96;47mS[0;1;33;93;5;42;102mX[0;1;36;96;5;42;102m8[0;1;32;92;5;43;103m8[0;1;32;92;5;46;106m8[0;1;32;92;5;43;103m8[0;1;32;92;5;46;106m8[0;1;33;93;47m@[0;37;5;42;102m8[0;32;5;42;102m%[0;1;32;92;42m8[0;32;5;42;102mt[0;37;5;42;102m888[0;1;33;93;5;42;102m8[0;1;32;92;5;46;106m8[0;1;32;92;5;43;103m8[0;1;32;92;5;46;106m8[0;37;5;43;103m8[0;1;36;96;5;42;102m8[0;32;5;42;102m@[0;1;30;90;42m8[0;1;30;90;40m@[0;32;40m. [0m\n"
"[0;34;40m [0;31;40m .[0;33;5;40;100mX[0;33;47m8[0;1;31;91;5;43;103m888[0;1;33;93;47m8[0;37;5;47;107m8:[0;1;37;97;47mX[0;37;5;43;103m8[0;37;5;47;107mS.8[0;37;5;43;103m8[0;37;5;47;107m S[0;1;33;93;5;41;101m8[0;37;5;43;103m%[0;1;33;93;5;41;101m8[0;1;31;91;5;43;103m8[0;1;33;93;5;41;101m8[0;1;31;91;5;43;103m8[0;37;5;43;103m8[0;31;5;40;100mS[0;31;40m.   [0;32;40m [0;31;40m.[0;1;30;90;40mS[0;34;40mt[0;31;40m;[0;32;40m:[0;1;30;90;40m@[0;36;5;40;100m.[0;37;5;40;100m@[0;1;30;90;47m888[0;35;5;40;100m.[0;33;5;40;100m%[0;34;5;40;100mS[0;1;30;90;47m88[0;37;5;40;100m8[0;1;30;90;47m8[0;35;5;40;100m [0;30;5;40;100mX[0;34;40m:[0;32;40m.[0;34;40m:[0;1;30;90;40mS[0;32;40m:[0;31;40m.[0;34;40m.[0;32;40m t[0;1;30;90;42m@[0;1;32;92;42m@[0;32;5;42;102m:[0;33;5;42;102m [0;37;5;42;102m88[0;1;36;96;5;42;102m@[0;1;32;92;5;43;103m8[0;1;36;96;5;42;102m8[0;1;33;93;5;42;102m8[0;1;36;96;5;42;102mX[0;33;5;42;102m [0;32;5;42;102m ; [0;37;5;42;102m8[0;1;33;93;5;42;102mS[0;1;36;96;5;42;102m8[0;1;33;93;5;42;102m8[0;1;36;96;5;42;102m8[0;1;33;93;5;42;102m8[0;1;36;96;5;42;102m@[0;1;33;93;5;42;102mX[0;37;5;42;102m8[0;32;5;42;102m [0;1;32;92;42m@[0;1;30;90;42m8[0;31;40m:[0;32;40m.[0m\n"
"[0;31;40m [0;34;40m.[0;33;5;40;100mt[0;33;5;43;103m [0;33;5;41;101m [0;37;5;43;103mX[0;1;31;91;5;43;103m8[0;1;33;93;5;41;101m8[0;37;5;43;103m88[0;1;33;93;47mS[0;37;5;47;107m% .;8;  t8[0;1;37;97;47mt[0;1;31;91;5;43;103m@[0;37;5;43;103m8[0;1;33;93;5;41;101m@8[0;37;5;43;103m@[0;33;5;40;100m.[0;31;40mS[0;32;40m.[0;34;40m :[0;1;30;90;40m8[0;32;40m:[0;31;40m:[0;1;30;90;40mX[0;36;5;40;100m [0;1;30;90;47mX[0;37;5;40;100m88[0;36;5;40;100m   [0;1;30;90;40mX[0;34;40m [0;32;40m.[0;33;5;40;100m.  [0;37;5;40;100mS[0;1;30;90;47m88[0;37;5;40;100m8[0;30;5;40;100m@[0;32;40m.[0;34;40m:[0;1;30;90;40mXS[0;32;40m [0;31;40m.[0;32;40m@[0;1;32;92;42m;[0;32;5;42;102m8;                     :[0;1;32;92;42m8[0;1;30;90;42m;[0;32;40mS[0;34;40m:[0m\n"
"[0;34;40m [0;1;30;90;40m8[0;1;33;93;47m8[0;33;5;41;101m [0;37;5;43;103mX[0;1;31;91;5;43;103m8[0;37;5;43;103m8[0;1;33;93;5;41;101m@88[0;1;31;91;5;43;103m8[0;37;5;47;107m:..8[0;1;31;91;5;43;103m88[0;37;5;43;103m8[0;37;5;47;107m8t;.%[0;1;33;93;5;41;101m8[0;37;5;43;103m@[0;1;33;93;5;41;101m8[0;1;31;91;5;43;103m8[0;37;5;43;103m8[0;35;5;40;100m8[0;31;40m .[0;30;5;40;100m8[0;32;40m:[0;34;40m:[0;32;40m:[0;37;5;40;100m8[0;1;30;90;47m@[0;37;5;40;100m8[0;1;30;90;47m8[0;37;5;40;100m@[0;34;40m.[0;32;40m.[0;34;40m::[0;32;40m:[0;34;40m:[0;32;40m.[0;34;40m.[0;32;40m.[0;30;5;40;100m8[0;1;30;90;47m@[0;37;5;40;100m88[0;1;30;90;47mX[0;30;5;40;100m8[0;32;40m [0;34;40m [0;1;30;90;40m8[0;32;40m:[0;34;40m:[0;32;40m8[0;1;30;90;42m%[0;32;5;42;102m8;                     ;[0;1;32;92;42m8[0;1;30;90;42m;[0;32;40m%[0;31;40m:[0m\n"
"[0;34;40m [0;33;5;40;100m;[0;1;31;91;5;43;103mX[0;37;5;43;103mS[0;1;33;93;5;41;101m8[0;1;31;91;5;43;103m8[0;1;33;93;5;41;101m8[0;37;5;43;103m8[0;1;33;93;5;41;101m8[0;37;5;43;103m88[0;37;5;47;107m :.[0;1;37;97;5;43;103m@[0;1;33;93;5;41;101m8[0;1;31;91;5;43;103m8[0;1;33;93;5;41;101m8[0;1;31;91;5;43;103m8[0;37;5;47;107m8 . [0;37;5;43;103m8[0;1;33;93;5;41;101m@[0;37;5;43;103m@[0;1;33;93;5;41;101m@[0;1;31;91;5;43;103m8[0;1;30;90;47m8[0;31;40m;:[0;1;30;90;40mS[0;34;40m.[0;31;40m:[0;36;5;40;100m [0;1;30;90;47m@[0;37;5;40;100m8[0;1;30;90;47m8[0;37;5;40;100m88[0;1;30;90;40m8888[0;30;5;40;100m8[0;31;40m:[0;34;40m:[0;32;40m;[0;31;40m:[0;36;5;40;100m.[0;1;30;90;47m8[0;37;5;40;100m8[0;1;30;90;47m8[0;37;5;40;100m88[0;1;30;90;40mS[0;34;40m.;[0;1;30;90;40m8[0;31;40m:[0;1;30;90;40m8[0;1;30;90;42m%[0;1;32;92;42m8[0;32;5;42;102mS.                    X[0;1;32;92;42mS[0;1;30;90;42m8[0;34;40mt[0;31;40m:[0m\n"
"[0;34;40m;[0;33;47m8[0;1;31;91;5;43;103m8[0;1;33;93;5;41;101m8[0;37;5;43;103m8[0;1;33;93;5;41;101m8[0;37;5;43;103m8[0;1;33;93;5;41;101m@[0;37;5;43;103m@[0;1;33;93;5;41;101mS[0;37;5;47;107m8@::8[0;1;33;93;5;47;107m8[0;37;5;47;107m8[0;1;33;93;5;47;107m8[0;37;5;47;107m8t.;8[0;1;33;93;5;41;101m8[0;1;31;91;5;43;103m8[0;1;33;93;5;41;101m8[0;37;5;43;103m8[0;1;33;93;5;41;101m8[0;37;5;43;103m8[0;31;40m%[0;34;40m:[0;32;40m;[0;34;40m:[0;30;5;40;100m@[0;1;30;90;47m@[0;37;5;40;100m8[0;1;30;90;47m8[0;37;5;40;100m8[0;1;30;90;47m8[0;37;5;40;100m8[0;1;30;90;47m888[0;37;5;40;100m@[0;1;30;90;40m@[0;32;40m:[0;34;40m:[0;1;30;90;40m@[0;35;5;40;100m [0;1;30;90;47m@[0;37;5;40;100m8[0;1;30;90;47m8[0;37;5;40;100m88[0;1;30;90;47m@[0;33;5;40;100m [0;32;40m.:[0;31;40m;[0;34;40m.[0;32;40m.[0;1;30;90;40mX[0;1;30;90;42mX[0;32;5;42;102m8X.                 .@[0;1;32;92;42m@[0;1;30;90;42m8[0;32;40m%[0;31;40m;[0;32;40m.[0m\n"
"[0;34;40mt[0;1;33;93;47m8[0;1;33;93;5;41;101m8[0;37;5;43;103mX[0;1;33;93;5;41;101m@[0;1;31;91;5;43;103m8[0;1;33;93;5;41;101m8[0;1;31;91;5;43;103m8[0;1;33;93;5;41;101m8[0;1;31;91;5;43;103m8[0;37;5;47;107m8;.;88X;t. [0;1;33;93;5;47;107m8[0;1;33;93;5;41;101m8[0;1;31;91;5;43;103m88[0;37;5;43;103m8[0;1;33;93;5;41;101m@[0;1;31;91;5;43;103m8[0;1;33;93;47m8[0;31;40m@[0;34;40m%[0;31;40mt[0;32;40m:[0;36;5;40;100m:[0;1;30;90;47m8[0;37;5;40;100m88[0;1;30;90;47m8[0;37;5;40;100m8[0;1;30;90;47m8[0;37;5;40;100m8[0;1;30;90;47m8[0;37;5;40;100mX[0;32;40m:[0;34;40m:[0;31;40m:[0;1;30;90;40mX[0;1;30;90;47m88[0;37;5;40;100m8[0;1;30;90;47m8[0;37;5;40;100m8[0;1;30;90;47m8[0;37;5;40;100m8[0;1;30;90;47m8[0;37;5;40;100m@[0;34;40m::[0;32;40m%[0;34;40m:[0;32;40m.:[0;1;30;90;40mX[0;1;30;90;42m8[0;32;5;42;102m88;               ;8[0;1;32;92;42mS[0;32;5;40;100m@[0;32;40mt[0;34;40m;[0;31;40m.[0;34;40m.[0m\n"
"[0;34;40m:[0;33;47m8[0;1;31;91;5;43;103m8[0;1;33;93;5;41;101m8[0;37;5;43;103m8[0;1;33;93;5;41;101m8[0;1;33;93;47m8[0;1;33;93;5;41;101m8[0;37;5;43;103m8[0;1;33;93;5;41;101m8[0;37;5;47;107mt  @[0;1;31;91;5;43;103m888[0;37;5;43;103m8[0;37;5;47;107m8S;t;[0;37;5;43;103m8[0;1;33;93;5;41;101m88[0;37;5;43;103m8[0;1;33;93;5;41;101m@[0;33;47m8[0;31;40m8[0;34;40mt[0;32;40mt[0;34;40m.[0;30;5;40;100mS[0;1;30;90;47m@[0;37;5;40;100m8[0;1;30;90;47m8[0;37;5;40;100m8[0;1;30;90;47m8[0;37;5;40;100m8[0;1;30;90;47m8[0;36;5;40;100m [0;34;40m;[0;31;40m;[0;32;40m:[0;30;5;40;100m8[0;37;5;40;100m@[0;1;30;90;47m8[0;37;5;40;100m8[0;1;30;90;47m8[0;37;5;40;100m88[0;1;30;90;47m8[0;37;5;40;100m8[0;1;30;90;47m8[0;33;5;40;100m [0;32;40m;[0;31;40m;[0;34;40m%[0;31;40m:[0;32;40m..:%[0;32;5;40;100m8[0;1;32;92;42mS8[0;32;5;42;102mS:           ;X[0;1;32;92;42m8[0;1;30;90;42m88[0;32;40m;[0;31;40m;[0;34;40m.[0;32;40m.[0;31;40m [0m\n"
"[0;34;40m [0;33;5;40;100m:[0;37;5;43;103m@[0;1;33;93;5;41;101m88[0;37;5;43;103m8[0;1;33;93;5;41;101m@[0;1;33;93;47m8[0;1;33;93;5;41;101m8[0;37;5;47;107m8;. [0;37;5;43;103m8[0;1;33;93;5;41;101m8[0;1;31;91;5;43;103m@8[0;1;33;93;5;41;101m8[0;1;31;91;5;43;103m8[0;37;5;47;107mX .;[0;37;5;43;103m8[0;1;33;93;5;41;101m8[0;37;5;43;103m8[0;1;33;93;5;41;101mX[0;37;5;43;103m@[0;1;30;90;45m8[0;31;40mS[0;32;40m%[0;31;40m%[0;34;40m;[0;32;40m;[0;37;5;40;100m8[0;1;30;90;47m8[0;37;5;40;100m88[0;1;30;90;47m8[0;37;5;40;100m8[0;30;5;40;100m8[0;32;40m:;[0;34;40m;[0;30;5;40;100mS[0;36;5;40;100m    [0;37;5;40;100mX[0;1;30;90;47m8[0;37;5;40;100m88[0;1;30;90;47m88[0;30;5;40;100mX[0;34;40m.;[0;1;30;90;40m@[0;32;40m.[0;31;40m.[0;34;40m [0;31;40m.[0;34;40m.[0;31;40m.[0;1;30;90;40mX[0;32;5;40;100m8[0;33;42mt[0;32;5;42;102m8X;:   ..:t@[0;1;32;92;42m8[0;1;30;90;42m88[0;32;40m;[0;34;40m:[0;31;40m.[0;34;40m.[0;32;40m [0;31;40m  [0m\n"
"[0;32;40m [0;31;40mS[0;33;47m@[0;1;31;91;5;43;103m88[0;1;33;93;5;41;101m8[0;37;5;43;103m8[0;37;5;47;107m  %  :8[0;1;33;93;5;47;107m8[0;37;5;47;107m8[0;1;37;97;5;43;103m8[0;37;5;47;107m88 . ;[0;1;31;91;5;43;103m88[0;1;33;93;5;41;101m8[0;1;31;91;5;43;103m8[0;1;33;93;47m@[0;30;41m@[0;32;40m:[0;34;40mt[0;30;5;40;100m8[0;34;40m.[0;31;40m.[0;30;5;40;100m8[0;1;30;90;47m8[0;37;5;40;100m88[0;1;30;90;47mX[0;36;5;40;100m:[0;31;40m [0;34;40m;[0;31;40m;[0;32;40m;[0;34;40m:[0;31;40m:[0;34;40m:[0;32;40m;[0;34;40m:[0;1;30;90;40m8[0;1;30;90;47mX[0;37;5;40;100m88[0;1;30;90;47m8[0;35;5;40;100m [0;34;40m.[0;32;40m:[0;1;30;90;40m@[0;31;40mt[0;32;40m:[0;34;40m.[0;31;40m [0;34;40m [0;31;40m [0;34;40m   [0;1;30;90;40mX[0;32;5;40;100m8[0;1;32;92;42m8[0;32;5;42;102m8S;:.:;S[0;1;32;92;42m8X[0;32;5;40;100mS[0;32;40m;[0;34;40m:[0;31;40m:[0;34;40m.  [0;31;40m   [0m\n"
"[0;32;40m [0;31;40m;[0;31;5;40;100m@[0;1;33;93;47m8[0;1;33;93;5;41;101m8[0;37;5;43;103m@[0;1;33;93;5;41;101mX[0;1;37;97;47m [0;1;37;97;5;43;103m8[0;37;5;47;107m88:.SXS:t:.:@[0;1;31;91;5;43;103mX8[0;1;33;93;47m@[0;1;33;93;5;41;101m8[0;1;33;93;47mX[0;35;41m8[0;32;40m;[0;31;40m:.[0;1;30;90;40mXX[0;34;40m..[0;36;5;40;100m%[0;37;5;40;100m88[0;1;30;90;47m8[0;35;5;40;100m.[0;1;30;90;40m@88[0;34;40m;[0;32;40m:[0;34;40m;[0;30;5;40;100m88[0;1;30;90;40m8[0;35;5;40;100mt[0;1;30;90;47m8[0;37;5;40;100m8[0;1;30;90;47m@[0;36;5;40;100m [0;34;40m;[0;31;40m:[0;32;40m;[0;30;5;40;100m8[0;34;40m:[0;32;40m.[0;31;40m  [0;34;40m     [0;32;40m.[0;34;40m:[0;32;5;40;100m8[0;1;30;90;42mS[0;1;32;92;42m8[0;32;5;42;102mX%t%8[0;1;32;92;42m@[0;1;30;90;42m8[0;1;30;90;40m8[0;32;40m:[0;34;40m.[0;31;40m.[0;34;40m [0;31;40m      [0m\n"
"[0;34;40m . S[0;1;30;90;47m8[0;1;31;91;5;43;103m888[0;1;33;93;5;41;101m88[0;1;37;97;5;43;103m@[0;37;5;47;107m%[0;1;33;93;5;47;107m8[0;1;33;93;5;41;101m8[0;37;5;47;107mX;[0;1;37;97;5;43;103m8[0;1;33;93;5;41;101m8[0;1;31;91;5;43;103m@@8888[0;37;5;43;103m8[0;1;31;91;47m8[0;1;30;90;41m8[0;32;40m.[0;34;40m..[0;32;40m.[0;34;40m [0;32;40m:[0;1;30;90;40mX[0;31;40m..t[0;36;5;40;100m.[0;37;5;40;100m8[0;1;30;90;47m88[0;37;5;40;100m8[0;1;30;90;47mS[0;30;5;40;100mX[0;31;40mt[0;1;30;90;40m8[0;37;5;40;100m8[0;1;30;90;47m8[0;37;5;40;100m88[0;1;30;90;47m8[0;33;5;40;100m [0;30;5;40;100m8[0;32;40m::[0;1;30;90;40mS[0;34;40m%[0;31;40m:[0;32;40m.[0;31;40m    [0;34;40m     [0;31;40m  [0;34;40m.[0;30;5;40;100m8[0;1;32;92;42mtX@X[0;1;30;90;42m8[0;1;30;90;40m8[0;32;40m;.[0;34;40m.[0;31;40m.[0;34;40m    [0;31;40m    [0m\n"
"[0;34;40m  [0;31;40m.tt[0;35;5;40;100mS[0;1;33;93;47m@[0;1;31;91;5;43;103m888[0;37;5;47;107m88[0;37;5;43;103m8[0;1;33;93;5;41;101m8[0;37;5;47;107m :[0;1;31;91;5;43;103m88[0;37;5;43;103m8[0;1;33;93;5;41;101m8[0;1;31;91;5;43;103m8[0;37;5;43;103m8[0;1;33;93;5;41;101m8[0;1;37;97;47m [0;35;5;41;101m8[0;32;40mt[0;34;40m. [0;31;40m   [0;34;40m .[0;31;40m.[0;1;30;90;40m@[0;34;40m:[0;32;40m;[0;34;40m:[0;31;40m:[0;36;5;40;100m%[0;37;5;40;100mS[0;1;30;90;47m@8%%S@88[0;36;5;40;100m [0;1;30;90;40mS[0;32;40m.[0;34;40m:[0;31;40m:[0;1;30;90;40m@[0;31;40m;[0;32;40m:.[0;31;40m     [0;34;40m      [0;31;40m [0;32;40m .[0;1;30;90;40m@[0;1;30;90;42m8:[0;32;5;40;100m8[0;32;40m;.[0;34;40m.[0;31;40m.[0;32;40m  [0;34;40m      [0;31;40m  [0m\n"
"[0;34;40m [0;31;40m    [0;34;40m.:[0;31;40mt%[0;34;40mS[0;1;30;90;41m8[0;35;5;40;100m8[0;31;5;40;100m@[0;33;5;40;100m:[0;1;30;90;45m8[0;1;31;91;43m8[0;35;5;40;100m [0;1;30;90;45m8[0;35;5;40;100m@[0;1;30;90;41mX[0;34;40mS[0;31;40m8S[0;32;40m.[0;31;40m.[0;34;40m.[0;31;40m       [0;34;40m [0;31;40m.[0;32;40m.[0;31;40m.;[0;1;30;90;40mXXS[0;34;40m;[0;32;40mt[0;31;40m;:[0;34;40m:[0;32;40m:[0;34;40mt[0;1;30;90;40mX@X[0;32;40mt[0;31;40m:[0;34;40m:[0;31;40m:[0;34;40m.[0;31;40m        [0;34;40m     [0;31;40m   .[0;34;40m.[0;31;40m..[0;32;40m.[0;34;40m [0;31;40m  [0;34;40m          [0m\n"
"[0;34;40m  [0;31;40m   :@[0;35;5;40;100m;[0;33;47m8[0;1;31;91;5;43;103m88888@[0;37;5;43;103m8[0;1;33;93;5;41;101m8[0;1;31;91;5;43;103m88[0;1;33;93;47mX[0;1;33;93;5;41;101m8[0;1;31;91;47mX[0;33;5;40;100m.[0;1;30;90;41mX[0;32;40mt[0;31;40m:[0;34;40m.[0;31;40m      [0;34;40m [0;32;40m.[0;34;40m;[0;1;30;90;40mX[0;32;40m;[0;34;40m:[0;31;40m.:[0;1;30;90;40m@8[0;30;5;40;100m@[0;36;5;40;100mS[0;30;5;40;100m88[0;1;30;90;40m@[0;32;40m;[0;34;40m:[0;32;40m.[0;34;40m;[0;1;30;90;40mX[0;32;40mt[0;34;40m;[0;32;40m:[0;31;40m.[0;34;40m.[0;31;40m      [0;34;40m     [0;31;40m    [0;32;40m%[0;32;5;40;100m8[0;34;40m:[0;31;40m:[0;34;40m.[0;32;40m.[0;34;40m [0;32;40m  [0;34;40m       [0;31;40m [0m\n"
";
