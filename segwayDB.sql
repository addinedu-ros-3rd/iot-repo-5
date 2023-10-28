-- DROP TABLE IF EXISTS `mpu6050`;
CREATE TABLE mpu6050
(
    id INT NOT NULL,
    time_stamp TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    acc_x FLOAT,
    acc_y FLOAT,
    acc_z FLOAT,
    gyro_x FLOAT,
    gyro_y FLOAT,
    gyro_z FLOAT,
    PRIMARY KEY (id)
);

-- DROP TABLE IF EXISTS `motor`;
CREATE TABLE motor
(
    id INT NOT NULL,
    time_stamp TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    direction VARCHAR(1),
    PRIMARY KEY (id)
);

-- DROP TABLE IF EXISTS `rfid_user`;
CREATE TABLE rfid_user
(
    id INT NOT NULL,
    uid VARCHAR(100),
    user_name VARCHAR(100),
    PRIMARY KEY (id)
);

-- DROP TABLE IF EXISTS `rfid_tag_log`;
CREATE TABLE rfid_tag_log
(
    id INT NOT NULL,
    time_stamp TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    uid VARCHAR(100),
    PRIMARY KEY (id)
);