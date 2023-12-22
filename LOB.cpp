#include "LOB.h"

LOB::LOB(int instrument): bid_tree(instrument, BID), ask_tree(instrument, ASK), instrument(instrument) {};

