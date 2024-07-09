import pygame

pygame.font.init()
def render_text(text, font, color, surface, x, y):
    textobj = font.render(text, True, color)
    textrect = textobj.get_rect()
    textrect.topleft = (x,y)
    surface.blit(textobj, textrect)

def update_ui(fog_density):
    screen = pygame.display.get_surface()
    #screen.fill((0,0,0))
    font = pygame.font.Font(None, 36)
    render_text(f"Fog Density; {fog_density:.2f}", font, (255,255,255), screen, 20, 20)
    pygame.display.update()