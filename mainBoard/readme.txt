UW ROBOMASTERS
EMBEDDED DOCUMENTATION
Author: Pico Premvuti | natatp [at] uw.edu

This document specifies documentation for hardware and software setup.

=== HARDWARE CONFIGURATION ===
820R Motor Controllers
    Motor Position    CAN ID    DIP SW[1:4]
    Front Left        0x201     1000
    Back Left         0x202     0100
    Back Right        0x203     1100
    Front Right       0x204     0010

=== PROTOCOL ===

**** TPZ -> Arduino ****
byte
0  1   header (0x7A)
2  3   cireq_pitch
4  5   cireq_yaw
6  7   cireq_fire
8  9   control_state
10 11  
12 13  rc_ch0
14 15  rc_ch1
16 17  rc_ch2
18 19  rc_ch3
20 21  rc_s1 | rc_s2
22 23  mouse_x
24 25  mouse_y
26 27  mouse_z
28 29  mouse_press_l | mouse_press_r
30 31  key_v

**** Arduino -> RPI ****
byte
0  1   header (0x7B)
2  3   
4  5   
6  7   
8  9   
10 11  
12 13  rc_ch0
14 15  rc_ch1
16 17  rc_ch2
18 19  rc_ch3
20 21  rc_s1 | rc_s2
22 23  mouse_x
24 25  mouse_y
26 27  mouse_z
28 29  mouse_press_l | mouse_press_r
30 31  key_v

**** RPI -> Arduino ****
byte
0  1   header (0x7C)
2  3   rpireq_drive
4  5   rpireq_strafe
6  7   rpireq_rotate
8  9   rpireq_pitch
10 11  rpireq_yaw
12 13  rpireq_fire
14 15  
16 17  
18 19  
20 21  
22 23  
24 25  
26 27  
28 29  
30 31  

**** Arduino -> TPZ ****
byte
0  1   header (0x7D)
2  3   rpireq_drive
4  5   rpireq_strafe
6  7   rpireq_rotate
8  9   rpireq_pitch
10 11  rpireq_yaw
12 13  rpireq_fire
14 15  
16 17  
18 19  
20 21  
22 23  
24 25  
26 27  
28 29  
30 31  

**** Arduino -> AUX ****
byte
0  1   header (0x7E)
2  3   cireq_pitch
4  5   cireq_yaw
6  7   cireq_fire
8  9   control_state
10 11  
12 13  rc_ch0
14 15  rc_ch1
16 17  rc_ch2
18 19  rc_ch3
20 21  rc_s1 | rc_s2
22 23  mouse_x
24 25  mouse_y
26 27  mouse_z
28 29  mouse_press_l | mouse_press_r
30 31  key_v
