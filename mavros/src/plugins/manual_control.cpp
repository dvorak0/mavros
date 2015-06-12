/**
 * @brief Manual Control plugin
 * @file manual_control.cpp
 * @author Zhenfei Yang<yangzhenfe0@gmail.com>
 *
 * @addtogroup plugin
 * @{
 */
/*
 * Copyright 2014,2015 Zhenfei Yang.
 *
 * This file is part of the mavros package and subject to the license terms
 * in the top-level LICENSE file of the mavros repository.
 * https://github.com/mavlink/mavros/tree/master/LICENSE.md
 */

#include <mavros/mavros_plugin.h>
#include <pluginlib/class_list_macros.h>

#include <mavros/ManualControl.h>

namespace mavplugin
{
/**
 * @brief Manual Control plugin
 */
class ManualControlPlugin : public MavRosPlugin
{
public:
    ManualControlPlugin() :
        mc_nh("~manual_control"),
        uas(nullptr)
    { };

    void initialize(UAS &uas_)
    {
        uas = &uas_;

        manual_control_pub = mc_nh.advertise<mavros::ManualControl>("manual", 10);
    };

    const message_map get_rx_handlers()
    {
        return
        {
            MESSAGE_HANDLER(MAVLINK_MSG_ID_MANUAL_CONTROL, &ManualControlPlugin::handle_manual_control),
        };
    }

private:
    ros::NodeHandle mc_nh;
    UAS *uas;

    ros::Publisher manual_control_pub;

    void handle_manual_control(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid)
    {
        mavlink_manual_control_t manual;
        mavlink_msg_manual_control_decode(msg, &manual);

        auto manual_control_msg = boost::make_shared<mavros::ManualControl>();

        manual_control_msg->header.stamp = ros::Time::now();
        manual_control_msg->x = manual.x;
        manual_control_msg->y = manual.y;
        manual_control_msg->z = manual.z;
        manual_control_msg->r = manual.r;

        manual_control_pub.publish(manual_control_msg);
    }
};
};  // namespace mavplugin

PLUGINLIB_EXPORT_CLASS(mavplugin::ManualControlPlugin, mavplugin::MavRosPlugin)

