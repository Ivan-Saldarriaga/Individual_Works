import env,sensors
import pygame
import math

environment = env.buildEnvironment((600,1200))
environment.originalMap = environment.map.copy()
laser = sensors.LaserSensor(100, environment.originalMap, uncertainty=(0.02, 0.0001))
environment.map.fill((0,0,0))
environment.infomap = environment.map.copy()
running  = True

while running:
    sensorON = False
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if pygame.mouse.get_focused():
            sensorON = True
        elif not pygame.mouse.get_focused():
            sensorON = False
    if sensorON:
        position = pygame.mouse.get_pos()
        laser.position = position
        sensor_data = laser.sense_obstacles()
        environment.dataStorage(sensor_data)
        environment.show_sensorData(laser)
    environment.map.blit(environment.infomap, (0,0))
    pygame.display.update()
