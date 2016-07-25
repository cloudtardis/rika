#Camera Gimbal with Servos And Remote Control
Do everything on Pi!!!

#Hardware BOM:

- Servo, x1, for beta rotation
- Stepper, x1, for alpha rotation
- limit switch, x1, for alpha calibration



# Communication Protocal USB SERIAL
### (key:value)
- a_rel : (+/- 50) //alpha rotation relative 
- a_abs : (0~360) //alpha rotation absolut
- b_rel : (+/- 50) //beta rotation relative
- b_abs : (0~180) //beta rotation absolute. 0=down
- irlight : (0,1) //IR light switch 0=off 1=on
- cal : (1) //begin calibration

