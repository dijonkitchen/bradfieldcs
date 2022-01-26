# ANSI control codes for text formatting
GRAY = '\033[90m'
BLUE = '\033[95m'
BOLD = '\033[1m'
ENDC = '\033[0m'

# Instruction set
LOAD_WORD  = 0x01
STORE_WORD = 0x02
ADD        = 0x03
SUB        = 0x04
ADDI       = 0x05
SUBI       = 0x06
JUMP       = 0x07
BEQZ       = 0x08
HALT       = 0xff

# Loads the word (2 bytes) starting at the given address
# in the memory. Assumes little-endian byte order.
def load_word(memory, addr):
    lo = memory[addr]
    hi = memory[addr + 1]
    return 256 * hi + lo

# Stores the word (2 bytes) starting at the given address
# in the memory, in little-endian byte order.
def store_word(memory, addr, word):
    lo = word % 256
    hi = word // 256
    memory[addr] = lo
    memory[addr + 1] = hi

def print_state(registers, memory):
    print('Current State')

    # Registers
    print()
    print(f'    r1: {registers[1]}')
    print(f'    r2: {registers[2]}')
    print()

    pc = registers[0]

    # Next instruction
    print(f'    pc: 0x{pc:02x} -- ', end='')
    if memory[pc] == HALT:
        print('halt')
    arg1 = memory[pc + 1]
    if memory[pc] == JUMP:
        print(f'jump {arg1:02x}')
    arg2 = memory[pc + 2]
    if memory[pc] == LOAD_WORD:
        print(f'load r{arg1} (0x{arg2:02x})')
    elif memory[pc] == STORE_WORD:
        print(f'store r{arg1} (0x{arg2:02x})')
    elif memory[pc] == ADD:
        print(f'add r{arg1} r{arg2}')
    elif memory[pc] == SUB:
        print(f'sub r{arg1} r{arg2}')
    elif memory[pc] == ADDI:
        print(f'addi r{arg1} {arg2}')
    elif memory[pc] == SUBI:
        print(f'subi r{arg1} {arg2}')
    elif memory[pc] == BEQZ:
        print(f'beqz r{arg1} {arg2}')

    # ASCII arrow to pc location
    print('    ' + GRAY + '-' * (3 * pc + 1) + 'v' + ENDC)

    # Memory
    print('    ' + GRAY + '00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 16 17' + ENDC)
    print('    ' + GRAY + '-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --' + ENDC)
    print('    ' + BOLD + ' '.join(map(lambda x: f'{x:02x}', memory)) + ENDC)
    print('    ' + GRAY + 'INSTRUCTIONS ---------------------------------------^ OUT-^ IN-1^ IN-2^' + ENDC)
    print()

# Simulate the virtual machine with the given memory, with the
# program counter starting at 0x00. The machine will continue
# the "fetch-decode-execute" loop until it reaches a "halt"
# instruction, at which point it will terminate.
def run_vm(memory):

    # Initialize register state
    # Note that unlike `memory`, each entry in the register can hold
    # a 16-bit value directly (up to 65535)
    registers = [
      0x0000, # program counter / instruction pointer
      0x0000, # general purpose register 1
      0x0000, # general purpose register 2
    ]

    while True:
        pc = registers[0]
        instruction = memory[pc]

        print_state(registers, memory)
        input()

        # Handle 0-arg instruction (halt) as special case
        if instruction == HALT:
            return

        arg1 = memory[pc + 1]

        # Handle 1-arg instruction (jump) as special case
        if instruction == JUMP:
            registers[0] = arg1
            continue

        arg2 = memory[pc + 2]

        # load_word reg (addr)
        if instruction == LOAD_WORD:
            registers[arg1] = load_word(memory, arg2)

        # store_word reg (addr)
        elif instruction == STORE_WORD:
            store_word(memory, arg2, registers[arg1])

        # add reg1 reg2
        elif instruction == ADD:
            registers[arg1] += registers[arg2]

        # sub reg1 reg2
        elif instruction == SUB:
            registers[arg1] -= registers[arg2]

        # addi reg x
        elif instruction == ADDI:
            registers[arg1] += arg2

        # subi reg x
        elif instruction == SUBI:
            registers[arg1] -= arg2

        # beqz reg offset
        elif instruction == BEQZ:
            if registers[arg1] == 0:
                registers[0] += arg2

        else:
            raise Exception('Unrecognized instruction:', instruction)

        # All 2-arg instructions advance the program counter
        # by the same amount.
        registers[0] += 3

if __name__ == '__main__':
    """
    00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 16 17
    __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
    INSTRUCTIONS ---------------------------------------^ OUT-^ IN-1^ IN-2^
    """
    memory = [
        0x01, 0x01, 0x14, # load_word r1 (0x14)
        0x08, 0x01, 0x08, # beqz r1 0x08
        0x03, 0x02, 0x01, # add r2 r1
        0x06, 0x01, 0x01, # subi r1 1
        0x07, 0x03,       # jump 3
        0x02, 0x02, 0x12, # store r2 (0x12)
        0xff,             # halt

        0x00, 0x00,       # output

        0x0a, 0x00,       # input 1
        0x00, 0x00        # input 2
    ]

    run_vm(memory)
