DROP TABLE IF EXISTS `mpu6050`;
CREATE TABLE mpu6050
(
    id INT NOT NULL,
    time_stamp TIMESTAMP NOT NULL,
    gyro_yaw DECIMAL(11, 5),
    gyro_pitch DECIMAL(11, 5),
    gyro_roll DECIMAL(11, 5),
    PRIMARY KEY (id)
);

DROP TABLE IF EXISTS `motor`;
CREATE TABLE motor
(
    id INT NOT NULL,
    time_stamp TIMESTAMP NOT NULL,
    direction VARCHAR(2),
    PRIMARY KEY (id)
);

DROP TABLE IF EXISTS `rfid_user`;
CREATE TABLE rfid_user
(
    id INT NOT NULL,
    uid VARCHAR(100),
    user_name VARCHAR(100),
    PRIMARY KEY (id)
);

DROP TABLE IF EXISTS `rfid_tag_log`;
CREATE TABLE rfid_tag_log
(
    id INT NOT NULL,
    time_stamp TIMESTAMP NOT NULL,
    uid VARCHAR(100),
    PRIMARY KEY (id)
);