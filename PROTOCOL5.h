//
// Created by mark on 11/20/2024.
//
#ifndef PROJECT_PROTOCOL5_H
#define PROJECT_PROTOCOL5_H
//----------------------SOME CONSTANTS---------------------------------//
#define PROPAGATION_DELAY 2
#define TIMER_TIMOUT 5
#define MAX_PKT 1024
#define MAX_SEQ 1e3

typedef unsigned int seq_nr;
typedef struct {unsigned char data[MAX_PKT];} packet ;
typedef enum {info,ack,nak, notframe} frame_kind;
typedef enum {zero, one} check_sum;
typedef struct{
    frame_kind kind= notframe ;
    seq_nr seq;
    seq_nr ack;
    packet info;
    check_sum check;
}Frame;
#define AT_RECEIVER second
#define AT_SENDER first
#define inc(k) if (k<MAX_SEQ) k++ else k=0

int cur_time;
int timer_start;
int no_of_frames_to_send ;
int seq_no_turn = 1 ;


#endif //PROJECT_PROTOCOL5_H
