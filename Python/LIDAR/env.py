import math
import pygame
import random

class buildEnvironment:
    def __init__(self, MapDimensions):
        pygame.init()
        self.pointCloud = []
        self.externalMap = pygame.image.load('map.png')
        self.maph, self.mapw = MapDimensions
        self.MapWindowName = 'RRT path planning'
        pygame.display.set_caption(self.MapWindowName)
        self.map = pygame.display.set_mode((self.mapw, self.maph))
        self.mapCOPY = self.map.copy()
        self.map.blit(self.externalMap, (0, 0))
        self.black = (0, 0, 0)
        self.grey = (70, 70, 70)
        self.Blue = (0, 0, 255)
        self.Green = (0, 255, 0)
        self.Red = (255, 0, 0)
        self.white = (255, 255, 255)

        # FOG BEHAVIOR
        self.fog_density = 0.0
        self.fog_texture = pygame.image.load('noise.png').convert_alpha()
        
        # RAIN BEHAVIOR
        self.rain = False

    def toggle_rain(self):
        self.rain = not self.rain

    def AD2pos(self, distance, angle, robotPosition):
        x = distance * math.cos(angle) + robotPosition[0]
        y = -distance * math.sin(angle) + robotPosition[1]
        return (int(x), int(y))

    def dataStorage(self, data):
        if data is not False:
            detected_points = []  # List to store detected points
            for element in data:
                point = self.AD2pos(element[0], element[1], element[2])
                detected_points.append(point)
            self.pointCloud = detected_points[:]  # Update pointCloud with detected points

    def get_fog_color(self):
        fog_color_value = int(100 * self.fog_density)  # Adjust the intensity of the fog color
        return (fog_color_value, fog_color_value, fog_color_value, 50)  # RGBA color with low alpha for transparency

    def get_color_based_on_distance(self, distance, max_range):
        # Example gradient from blue to red based on distance
        if distance <= max_range:
            normalized_distance = distance / max_range
            blue = int(255 * (1 - normalized_distance))
            red = int(255 * normalized_distance)
            return (red, 0, blue)
        else:
            # Handle distances greater than max_range (optional: return black for now)
            return (0, 0, 0)

    def show_sensorData(self, laser):
        self.infomap = pygame.Surface((self.mapw, self.maph), pygame.SRCALPHA)
        
        for point in self.pointCloud:
            distance = math.sqrt((point[0] - laser.position[0]) ** 2 + (point[1] - laser.position[1]) ** 2)
            if self.rain:
                # Add random noise to distance (simulate rain effect)
                distance += random.uniform(-50, 50)  # Adjust the range as needed
            
            color = self.get_color_based_on_distance(distance, 100)
            
            # Ensure color values are within valid range (0-255)
            color = tuple(max(0, min(255, c)) for c in color)
            
            self.infomap.set_at(point, color)

        # Draw fog effect on the infomap
        fog_surface = pygame.Surface((self.mapw, self.maph), pygame.SRCALPHA)
        fog_surface.fill((0, 0, 0, int(255 * self.fog_density)))  # Black fog with varying alpha based on fog_density
        self.infomap.blit(fog_surface, (0, 0))

        # Clear map to black background if fog density is zero
        if self.fog_density == 0:
            self.map.fill((0, 0, 0))
        
        # Draw infomap on the main map surface
        self.map.blit(self.infomap, (0, 0))
        
        pygame.display.update()

    def increase_fog_density(self):
        self.fog_density += 0.1  # Example increase step, adjust as needed
        if self.fog_density > 1.0:
            self.fog_density = 1.0  # Cap fog density at maximum

    def decrease_fog_density(self):
        self.fog_density -= 0.1  # Example decrease step, adjust as needed
        if self.fog_density < 0.0:
            self.fog_density = 0.0  # Ensure fog density does not go below 0.0
