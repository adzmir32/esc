# Motor mapping and direction on the frame

ArduCopter's Quad X needs motor 1 at the front right, 2 at the rear left, 3 at the front left and 4 at the rear right, with 1 and 2 turning counter-clockwise (CCW) and 3 and 4 clockwise (CW). The motors on this frame sit in different corners than the ESC outputs assume, so the FC outputs were remapped in parameters instead of moving the motors. Front is the top of the frame as the operator described it.

| Frame corner | ESC chip | FC output | Parameter set | Becomes motor | Spin needed | Motor Test letter |
| --- | --- | --- | --- | --- | --- | --- |
| Front right | U9 | 4 | `SERVO4_FUNCTION` = 33 | 1 | CCW | A |
| Rear right | U7 | 3 | `SERVO3_FUNCTION` = 36 | 4 | CW | B |
| Rear left | U2 | 1 | `SERVO1_FUNCTION` = 34 | 2 | CCW | C |
| Front left | U5 | 2 | `SERVO2_FUNCTION` = 35 | 3 | CW | D |

The Motor Test runs A to D in a circle (front right, rear right, rear left, front left), which confirms the mapping. All motors turn CW by default, so the two CCW motors use the Reversed setting in am32.ca.

## am32.ca card numbers

The cards do not follow the output order. This table was built from the motor-test results and gave the correct spin on all four motors:

| Card | Drives | Reversed |
| --- | --- | --- |
| 1 | U7 (rear right) | off |
| 2 | U2 (rear left) | on |
| 3 | U5 (front left) | off |
| 4 | U9 (front right) | on |

Propellers: CCW props on the front right and rear left motors, CW props on the rear right and front left motors.

## FC settings used

`MOT_PWM_TYPE` = 5 (DShot300), `SERVO_DSHOT_ESC` = 1, `SERVO_BLH_AUTO` = 1, `SERVO_BLH_POLES` = 14. `SERVO_BLH_TEST` and `SERVO_BLH_DEBUG` should be 0.

## Rules learned

- After saving direction in am32.ca, switch off both the ESC and FC supplies for about 30 seconds before testing. A short ESC power-cycle was not enough.
- Test with props off, at 5 to 10% throttle for 2 seconds each, through a fuse and series resistor first.
- Other ESC settings were left at the values read back: poles 14, KV 1340, startup power 100%, timing 15 degrees, variable PWM, low-voltage cut-off off, temperature limit disabled, current limit 0.
