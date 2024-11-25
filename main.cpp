#include <bits/stdc++.h>
#include "PROTOCOL5.h"
using namespace std ;
/**
Description : local variable "frame" keeps getting updated when passed between layers

 THE LINES CARRYING DATA IS REPRESENTED BY QUEUES

**/
int expected_to_be_received =0;
vector<pair<Frame,Frame>> timeline(1e4); // (frames at sender ,  frames at reciver ) //models the physical wire



void to_physical_layer_to_receiver(Frame frame) {
    //TODO: introduce error here
    timeline[cur_time+PROPAGATION_DELAY].AT_RECEIVER=frame;
}
void to_physical_layer_to_sender(Frame frame) {
    //TODO : introduce error here
    timeline[cur_time+PROPAGATION_DELAY].AT_SENDER =frame;
}
void from_physical_layer_at_sender(Frame &frame) {
    frame = timeline[cur_time].AT_SENDER ;
}
void from_physical_layer_at_receiver(Frame &frame) {
    frame = timeline[cur_time].AT_RECEIVER ;
}


void to_network_layer_to_receiver(packet pack) {

}

void from_network_layer_at_sender(packet &pack) {

}



bool network_is_ready();
bool physical_to_sender_ready();
bool physical_to_receiver_ready();


void increase (seq_nr &x)
{
x++;
x=x%(MAX_SEQ+1);
}


queue<pair<int,seq_nr>>timers;
void start_timer (seq_nr s)
{
    timers.push(make_pair(cur_time,s));
}
bool epected_ack(seq_nr r)
{
  return  timers.front().second==r;
}

void send(seq_nr f_n,packet f_info)
{
 Frame s;
 s.seq=f_n;
 s.info=f_info;
    to_physical_layer_to_sender(s);
    start_timer(s.seq);
}

void network(){
    //
}
packet buffer[MAX_SEQ+1];//the packets buffered at sender
int nbuffer;    //size of window at sender
seq_nr next_send; // sequence of the next send frame
void sender(){

    if (network_is_ready())
    {
        from_network_layer_at_sender(buffer[next_send]);
        nbuffer++;
        send(next_send,buffer[next_send]);
        increase(next_send);
    }

    if(physical_to_receiver_ready())
    {
        Frame r;
        from_physical_layer_at_sender(r);
        if(epected_ack(r.ack))
        {

        }
    }


}
void receiver (){
    // yous was here
    static int Error_Discard_upcoming_frames=0;
    Frame received_frame;
    Frame feedback;
    from_physical_layer_at_receiver(received_frame); //received_frame now is updated by physical layer
    if(received_frame.kind == info )
        {
            if(received_frame.seq == expected_to_be_received && received_frame.check == 1){ //wire pair (sent, ack)
                //send ack
                cout<<"Ack:"<<received_frame.seq<<endl;
                Error_Discard_upcoming_frames=0;
                expected_to_be_received++;
                feedback.kind=ack;
                feedback.ack = received_frame.seq;
                to_physical_layer_to_sender(feedback);
            }
            else if(received_frame.seq != expected_to_be_received || received_frame.check == 0) // wire pair (sent,discard)
            {
                cout<<"Discarded"<<endl;
                Error_Discard_upcoming_frames=1;
            }
            else if(Error_Discard_upcoming_frames == 1) // wire pair (sent,nack)
            {
                cout<<"Nack:"<<received_frame.seq<<endl;
                feedback.kind = nak;
                feedback.ack = received_frame.seq;
                to_physical_layer_to_sender(feedback);
            }
        }
    else if (received_frame.kind != info)
        {
            cout<<"No frames received"<<endl; //for testing
        }
}
int main() {
    cout << "PLEASE enter the number of frames you want to send\n";
    cin >> no_of_frames_to_send;
    for (cur_time = 0; cur_time < 1e6; ++cur_time) { // each step is a unit of time
        sender();
        receiver();
    }

    return 0;
}
