import pygame
import random

class RainManager:
    def __init__(self, surface):
        self.surface = surface
        self.raindrops = []  # List to store active raindrops
        self.rain_active = False  # Flag to toggle rain simulation

    def toggle_rain(self):
        self.rain_active = not self.rain_active

    def is_rain_active(self):
        return self.rain_active

    def update(self):
        if self.rain_active:
            # Add new raindrops randomly
            if random.random() < 0.2:  # Adjust frequency of raindrops as needed
                x = random.randint(0, self.surface.get_width())
                y = 0
                speed = random.randint(5, 15)  # Adjust raindrop speed as needed
                self.raindrops.append((x, y, speed))

            # Update positions of existing raindrops
            updated_raindrops = []
            for drop in self.raindrops:
                x, y, speed = drop
                y += speed
                # Remove raindrops that have fallen off the screen
                if y < self.surface.get_height():
                    updated_raindrops.append((x, y, speed))
            self.raindrops = updated_raindrops

    def draw(self):
        for drop in self.raindrops:
            x, y, _ = drop
            pygame.draw.circle(self.surface, (100, 100, 255), (x, y), 2)  # Adjust raindrop appearance as needed

    def clear_raindrops(self):
        self.raindrops = []

