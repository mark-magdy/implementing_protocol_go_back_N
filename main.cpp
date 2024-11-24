#include <bits/stdc++.h>
#include "PROTOCOL5.h"
using namespace std ;
/**
Description : local variable "frame" keeps getting updated when passed between layers

**/
int expected_to_be_received =0;
vector<pair<Frame,Frame>> timeline(1e4); // (frames at sender ,  frames at reciver ) //models the physical wire

void to_physical_layer_to_receiver(Frame &frame) {
    //TODO: introduce error here
    timeline[cur_time+PROPAGATION_DELAY].AT_RECEIVER=frame;
}
void to_physical_layer_to_sender(Frame &frame) {
    //TODO : introduce error here
    timeline[cur_time+PROPAGATION_DELAY].AT_SENDER =frame;
}
void from_physical_layer_at_sender(Frame &frame) {
    frame = timeline[cur_time].AT_SENDER ;
}
void from_physical_layer_at_receiver(Frame &frame) {
    frame = timeline[cur_time].AT_RECEIVER ;
}

void network(){
    //
}
void sender(){
    //kirooo
    int x;
}
void receiver (){
    // yous was here
    static int Error_Discard_upcoming_frames=0;
    Frame received_frame;
    Frame feedback;
    from_physical_layer_at_receiver(&received_frame); //received_frame now is updated by physical layer
    if(received_frame.frame_kind == info )
        {
            if(received_frame.seq == expected_to_be_received && received_frame.check == 1){ //wire pair (sent, ack)
                //send ack
                cout<<"Ack:"<<received_frame.seq<<endl;
                Error_Discard_upcoming_frames=0;
                expected_to_be_received++;
                feedback.kind=ack;
                feedback.ack = received_frame.seq;
                to_physical_layer_to_sender(&feedback);
            }
            else if(received_frame.seq != expected_to_be_received || received_frame.check == 0) // wire pair (sent,discard)
            {
                cout<<"Discarded"<<endl;
                Error_Discard_upcoming_frames=1;
            }
            else if(Error_Discard_upcoming_frames == 1) // wire pair (sent,nack)
            {
                cout<<"Nack:"<<received_frame.seq<<endl;
                feedback.kind = nack;
                feedback.ack = received_frame.seq;
                to_physical_layer_to_sender(&feedback);
            }
        }
    else if (received_frame.frame_kind != info)
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
