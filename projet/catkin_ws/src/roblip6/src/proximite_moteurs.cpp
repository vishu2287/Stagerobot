#include <ros/ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int16MultiArray.h>
#include <sensor_msgs/Range.h>
#include <iostream>

using namespace std;

ros::Publisher pubgauche;
ros::Publisher pubdroite;
ros::Subscriber subrange;

void rangeCallBack( const std_msgs::Int16MultiArray &range_msg){
	std_msgs::Int16 vit_g;    //initialiser vitesse gauche
	std_msgs::Int16 vit_d;

	if(range_msg.data[0] < 8 && range_msg.data[1] > 10){
		vit_g.data = 0;
		vit_d.data = 32;
	}
	
	else if(range_msg.data[2] < 8 && range_msg.data[1] > 10){
		vit_g.data = 32;
		vit_d.data = 0;
	}
	
	else if(range_msg.data[1] < 5){
		vit_g.data = 0;
		vit_d.data = 0;
	}

	else if(range_msg.data[0] > 10 && range_msg.data[1] > 10 && range_msg.data[2] > 10){
		vit_g.data = 32;
		vit_d.data = 32;
	}

	else{
		vit_g.data = 15;
		vit_d.data = 15;
	}

	ROS_INFO("advertise the info gauche %d, droite %d", vit_g.data, vit_d.data);
	ros::Rate loop_rate(10);
	while(ros::ok()){
		pubgauche.publish(vit_g);
		pubdroite.publish(vit_d);
		ros::spin();
		loop_rate.sleep();
	}
}


int main(int argc, char** argv){
	ros::init(argc,argv,"proximite_moteurs_node");
	ros::NodeHandle nh;
	pubgauche = nh.advertise<std_msgs::Int16>("moteurgauche",10);
	pubdroite = nh.advertise<std_msgs::Int16>("moteurdroite",10);
	subrange = nh.subscribe("range_data",10,&rangeCallBack);
	ros::spin();
}
