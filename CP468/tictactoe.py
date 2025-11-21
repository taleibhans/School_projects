# Enhanced version of the Tic Tac Toe game with:
# 1. Sound effects
# 2. Turn indicator
# 3. Highlighted winning line

import pygame
import sys
import os
import random

# Initialize pygame
pygame.init()
pygame.mixer.init()

# Screen dimensions
WIDTH, HEIGHT = 600, 600
SQUARE_SIZE = WIDTH // 3
CIRCLE_RADIUS = SQUARE_SIZE // 3
CIRCLE_WIDTH = 15
CROSS_WIDTH = 20
SPACE = 30

# Colors
BG_COLOR = (56, 61, 59)
LINE_COLOR = (100, 105, 103)
CIRCLE_COLOR = (71, 221, 92)
CROSS_COLOR = (221, 71, 82)
HIGHLIGHT_COLOR = (255, 215, 0)

# Load sounds (make sure these files are in the project folder)
click_sound = pygame.mixer.Sound('click.wav')
win_sound = pygame.mixer.Sound('win.wav')
draw_sound = pygame.mixer.Sound('draw.wav')

# Initialize screen
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption('Tic-Tac-Toe Enhanced')

# Game state
board = [[None] * 3 for _ in range(3)]
current_player = 'X'
winner = None
winning_cells = []

def draw_grid():
    for row in range(1, 3):
        pygame.draw.line(screen, LINE_COLOR, (0, row * SQUARE_SIZE), (WIDTH, row * SQUARE_SIZE), 10)
        pygame.draw.line(screen, LINE_COLOR, (row * SQUARE_SIZE, 0), (row * SQUARE_SIZE, HEIGHT), 10)

def draw_figures():
    for row in range(3):
        for col in range(3):
            if board[row][col] == 'O':
                pygame.draw.circle(screen, CIRCLE_COLOR, (col * SQUARE_SIZE + SQUARE_SIZE // 2, row * SQUARE_SIZE + SQUARE_SIZE // 2), CIRCLE_RADIUS, CIRCLE_WIDTH)
            elif board[row][col] == 'X':
                pygame.draw.line(screen, CROSS_COLOR, (col * SQUARE_SIZE + SPACE, row * SQUARE_SIZE + SPACE),
                                 (col * SQUARE_SIZE + SQUARE_SIZE - SPACE, row * SQUARE_SIZE + SQUARE_SIZE - SPACE), CROSS_WIDTH)
                pygame.draw.line(screen, CROSS_COLOR, (col * SQUARE_SIZE + SPACE, row * SQUARE_SIZE + SQUARE_SIZE - SPACE),
                                 (col * SQUARE_SIZE + SQUARE_SIZE - SPACE, row * SQUARE_SIZE + SPACE), CROSS_WIDTH)

    # Highlight winning cells
    if winning_cells:
        for (r, c) in winning_cells:
            pygame.draw.rect(screen, HIGHLIGHT_COLOR, (c * SQUARE_SIZE, r * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE), 5)

def draw_turn_indicator():
    font = pygame.font.SysFont(None, 40)
    text = f"{current_player}'s Turn"
    img = font.render(text, True, (255, 255, 255))
    screen.blit(img, (20, 20))

def is_winner(player):
    global winning_cells
    for i in range(3):
        if all(cell == player for cell in board[i]):
            winning_cells = [(i, j) for j in range(3)]
            return True
    for j in range(3):
        if all(board[i][j] == player for i in range(3)):
            winning_cells = [(i, j) for i in range(3)]
            return True
    if all(board[i][i] == player for i in range(3)):
        winning_cells = [(i, i) for i in range(3)]
        return True
    if all(board[i][2 - i] == player for i in range(3)):
        winning_cells = [(i, 2 - i) for i in range(3)]
        return True
    return False

def is_draw():
    return all(cell is not None for row in board for cell in row)

def reset_game():
    global board, current_player, winner, winning_cells
    board = [[None] * 3 for _ in range(3)]
    current_player = 'X'
    winner = None
    winning_cells = []

def main():
    global current_player, winner
    clock = pygame.time.Clock()
    running = True

    while running:
        screen.fill(BG_COLOR)
        draw_grid()
        draw_figures()
        draw_turn_indicator()
        pygame.display.update()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.MOUSEBUTTONDOWN and winner is None:
                x, y = event.pos
                row, col = y // SQUARE_SIZE, x // SQUARE_SIZE
                if board[row][col] is None:
                    board[row][col] = current_player
                    click_sound.play()

                    if is_winner(current_player):
                        winner = current_player
                        win_sound.play()
                    elif is_draw():
                        draw_sound.play()
                    else:
                        current_player = 'O' if current_player == 'X' else 'X'
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_r:
                    reset_game()

        clock.tick(60)

    pygame.quit()
    sys.exit()

if __name__ == '__main__':
    main()
