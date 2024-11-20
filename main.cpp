#include <bits/stdc++.h>
#include "PROTOCOL5.h"
using namespace std ;

vector<pair<Frame,Frame>> timeline(1e4); // (frames at sender ,  frames at reciver )

void to_physical_layer_to_receiver(Frame &frame) {
    timeline[cur_time+PROPAGATION_DELAY].AT_RECEIVER=frame;
}
void to_physical_layer_to_sender(Frame &frame) {
    timeline[cur_time+PROPAGATION_DELAY].AT_SENDER =frame;
}
void from_physical_layer_at_sender(Frame &frame) {
    frame = timeline[cur_time].AT_SENDER ;
}
void from_physical_layer_at_receiver(Frame &frame) {
    frame = timeline[cur_time].AT_RECEIVER ;
}
void sender(){
    Frame r,s;
    from_physical_layer_at_sender(r);
    if ((r.ack==ack || r.kind==notframe) && seq_no_turn <= no_of_frames_to_send){
        s.seq=seq_no_turn++;
        s.kind=info;
        cout <<"sending frame with seq no"<< s.seq << endl;
        to_physical_layer_to_receiver(s);
    }
}
void receiver (){
    Frame r,s;
    from_physical_layer_at_receiver(r);
    if (r.kind==info) {
        s.ack = true;
        s.seq = r.seq;
        s.kind = ack;
        cout << "ACKN frame no " << r.seq << endl;
        to_physical_layer_to_sender(s);
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