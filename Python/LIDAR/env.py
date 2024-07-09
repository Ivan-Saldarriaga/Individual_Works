import math
import pygame

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
        self.map.blit(self.externalMap, (0,0))
        self.black = (0,0,0)
        self.grey = (70,70,70)
        self.Blue = (0,0,255)
        self.Green = (0,255,0)
        self.Red = (255,0,0)
        self.white = (255,255,255)

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

    def get_color_based_on_distance(self, distance, max_range):
        # Example gradient from blue to red based on distance
        if distance <= max_range:
            normalized_distance = distance / max_range
            blue = int(255 * (1 - normalized_distance))
            red = int(255 * normalized_distance)
            return (red, 0, blue)
        else:
            return (0, 0, 0)  # Set to black if distance exceeds max_range

    def show_sensorData(self, laser):
        self.infomap = self.map.copy()
        current_points = set()  # Track currently sensed points

        # Clear infomap to background map
        self.infomap.blit(self.mapCOPY, (0, 0))

        for point in self.pointCloud:
            distance = math.sqrt((point[0] - laser.position[0])**2 + (point[1] - laser.position[1])**2)
            if distance <= laser.Range:
                color = self.get_color_based_on_distance(distance, 100)
                self.infomap.set_at(point, color)
                current_points.add(point)

        # Update pointCloud to include only currently sensed points
        self.pointCloud = list(current_points)

        pygame.display.update()
