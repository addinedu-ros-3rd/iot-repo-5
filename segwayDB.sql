DROP TABLE IF EXISTS `mpu6050`;
CREATE TABLE mpu6050
(
    id INT NOT NULL AUTO_INCREMENT,
    time_stamp TIMESTAMP NOT NULL,
    acc_x INT,
    acc_y INT,
    acc_z INT,
    gyro_x INT,
    gyro_y INT,
    gyro_z INT,
    PRIMARY KEY (id)
);

DROP TABLE IF EXISTS `motor`;
CREATE TABLE motor
(
    id INT NOT NULL AUTO_INCREMENT,
    time_stamp TIMESTAMP NOT NULL,
    direction VARCHAR(2),
    PRIMARY KEY (id)
);

DROP TABLE IF EXISTS `user`;
CREATE TABLE user
(
    id INT NOT NULL AUTO_INCREMENT,
    uid VARCHAR(100),
    name VARCHAR(100),
    sign_in_date DATE,
    PRIMARY KEY (id)
);

DROP TABLE IF EXISTS `rfid_tag_log`;
CREATE TABLE rfid_tag_log
(
    id INT NOT NULL AUTO_INCREMENT,
    time_stamp TIMESTAMP NOT NULL,
    uid VARCHAR(100),
    PRIMARY KEY (id)
);