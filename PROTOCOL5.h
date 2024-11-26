//
// Created by mark on 11/20/2024.
//
#ifndef PROJECT_PROTOCOL5_H
#define PROJECT_PROTOCOL5_H
//----------------------SOME CONSTANTS---------------------------------//

#define MAX_PKT 1024
#define MAX_SEQ 1000

#include <string>
#include <queue>

using namespace std;

typedef unsigned int seq_nr;
typedef string packet;

typedef enum {
    info, ack, nak, notframe
} frame_kind;
typedef enum {
    zero, one
} check_sum;
typedef struct {
    frame_kind kind = notframe;
    seq_nr seq;
    seq_nr ack;
    packet info;
    check_sum check;
} Frame;

#define AT_RECEIVER second
#define AT_SENDER first

int cur_time;
int no_of_frames_to_send;
int PROPAGATION_DELAY = 1;
int TIMER_TIMOUT = 5;
int expected_to_be_received = 0;

//data for sender
packet buffer[MAX_SEQ + 1];//the packets buffered at sender
int nbuffer;    //size of window at sender
seq_nr next_send; // sequence of the next send frame
seq_nr ack_epected;
map<seq_nr, int> timers;
int retransmissions = 0;

// queue for network layer
queue<packet> all_packets_network;
queue<packet> all_packets_reciver;
// data for network layer
int frame_time;
vector<packet> data_from_input;
packet input;
bool stopNetwork = false;

#endif //PROJECT_PROTOCOL5_H
