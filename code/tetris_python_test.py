from tetris_ai_test import *
import re
import numpy as np


def get_all_you_want(ground, num_lines=30):
    ground_four = ground[:num_lines]
    matrix = np.asarray(list('0' * num_lines + ground[num_lines:])).reshape(20, 10).astype(np.bool)
    try: block = '1' + str(re.findall(r"1(.*)1", ground_four)[0]) +'1'
    except IndexError: return 'N', None, '100', '0'

    I = '1111'
    J = '1' + '0' * 9 + '1' * 3
    L = '1' + '0' * 7 + '1' * 3
    O = '1' * 2 + '0' * 8 + '1' * 2
    S = '1' * 2 + '0' * 7 + '1' * 2
    Z = '1' * 2 + '0' * 9 + '1' * 2
    T = '1' + '0' * 8 + '1' * 3

    shapes_with_dir = {I: 'I||1||5', J: 'J||2||4', L: 'L||0||4', O: 'O||0||4', S: 'S||0||4', T: 'T||1||4', Z: 'Z||0||4'}
    shape_station = shapes_with_dir.get(block)
    #print(matrix)
    try:                   shape, station, center = shape_station.split('||')
    except AttributeError: shape, station, center = 'N', '100', '0'
    return shape, matrix, int(center), int(station)


def get_best_point(shape, matrix):
    ai = Tetris_AI(shape, matrix)
    best_point = ai.getBestPoint()
    return best_point['center'][1], best_point['station']


def control(shape, ini_center, ini_station, center, station):
    shape_num = {'I': 2, 'J': 4, 'L': 4, 'O': 1, 'S': 2, 'T': 4, 'Z': 2}
    turn = (station-ini_station) % shape_num[shape]
    right_step = center-ini_center
    if right_step >= 0: return turn * '3' +   right_step  * '2' + (8-turn-right_step) * '0'
    else:               return turn * '3' + (-right_step) * '1' + (8-turn+right_step) * '0'


def main_zpd(cinput):
    shape, matrix, ini_center, ini_station = get_all_you_want(cinput)
    #print('shape: ', shape, 'ini_center: ',ini_center,'ini_station: ',ini_station)
    if shape == 'N': return '00000000'
    center, station = get_best_point(shape, matrix)
    #print('last_center: ',center,'last_station:',station)
    return control(shape, ini_center, ini_station, center, station)


if __name__ == "__main__":
    import time
    start = time.time()
    s = '0001111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000' \
            '0000000000000000000000000000000000000100000010010000111001010011111111101111111101'
    print(main_zpd(s))
    print(time.time()-start)