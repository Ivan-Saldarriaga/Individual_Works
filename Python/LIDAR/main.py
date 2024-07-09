import pygame
import math
import env
import sensors
import ui
from rainmanager import RainManager  # Import RainManager class

pygame.init()

# Initialize environment, laser sensor, UI
environment = env.buildEnvironment((600, 1200))
environment.originalMap = environment.map.copy()
laser = sensors.LaserSensor(100, environment.originalMap, uncertainty=(0.02, 0.0001))
environment.map.fill((0, 0, 0))
environment.infomap = environment.map.copy()

# Initialize rain manager
rain_manager = RainManager(environment.map)  # Pass the map surface to RainManager

running = True
sensorON = False

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_UP:
                environment.increase_fog_density()
            elif event.key == pygame.K_DOWN:
                environment.decrease_fog_density()
            elif event.key == pygame.K_r:  # Toggle rain simulation on 'r' key press
                rain_manager.toggle_rain()
                environment.toggle_rain()

        if pygame.mouse.get_focused():
            sensorON = True
        else:
            sensorON = False

    if sensorON:
        position = pygame.mouse.get_pos()
        laser.position = position
        sensor_data = laser.sense_obstacles()
        environment.dataStorage(sensor_data)
        environment.show_sensorData(laser)

    environment.map.blit(environment.infomap, (0, 0))
    ui.update_ui(environment.fog_density)

    # Update rain simulation if active
    if rain_manager.is_rain_active():
        rain_manager.update()

    # Draw raindrops if rain simulation is active
    if rain_manager.is_rain_active():
        rain_manager.draw()

    pygame.display.update()

pygame.quit()
