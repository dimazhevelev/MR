from reprint import output
from MRSim import MRBrain
import bios
import argparse
from pathlib import Path

if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    parser.add_argument('-i', type=str,
                        help='path of yaml file', default=None, required=True)

    args = parser.parse_args()

    my_file = Path(args.i)
    if my_file.exists() == False:
        print(f"File {args.i} doesn't exist, aborting")
        exit()
    brain = MRBrain()


    configuration = bios.read(args.i)
    if 'motors' not in configuration:
        print(f"Motors not present")
        exit()
    if 'positions' not in configuration:
        print(f"Positions not present")
        exit()
    if 'functions' not in configuration:
        print(f"Functions not present")
        exit()
    if len(configuration['motors']) != len(configuration['positions']):
        print(f"Motors count should match positions count")
        exit()
    for function in configuration['functions']:
        if brain.addNetFunction(function['funcName'], f"{function['funcName']}_{function['funcObject']}", function['funcObject']) != 0:
            print(f"Function {function['funcName']} was not registered")
    for motor in configuration['motors']:
        brain.addMotor(int(motor))
    brain.initPositions(configuration['positions']);
    brain.initialize()
    print("Hit Ctrl-C to exit")
    try:
        with output(initial_len=2, interval=0) as output_lines:
            while True:
                trunc_positions = []
                for item in brain.getPositions():
                    trunc_positions.append("%.6f"%item)
                output_lines[0] = f"Positions :  {trunc_positions}"
                output_lines[1] = f"Motors :  {brain.getMotorPositions()}"
    except KeyboardInterrupt:
        pass
    brain.deinitialize()
