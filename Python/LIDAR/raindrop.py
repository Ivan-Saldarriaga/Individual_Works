import pygame
import random
import time

class Raindrop:
    def __init__(self, screen_width, screen_height):
        self.x = random.randint(0, screen_width)
        self.y = random.randint(0, screen_height)
        self.appear_time = time.time()  # Time when raindrop appeared
        self.lifetime = random.uniform(0.5, 2.0)  # Random lifetime for raindrop
        self.color = (100, 100, 255)  # Color of raindrops

    def is_alive(self):
        return time.time() - self.appear_time < self.lifetime

    def draw(self, surface):
        pygame.draw.circle(surface, self.color, (self.x, self.y), 2)  # Draw raindrop as a small circle
