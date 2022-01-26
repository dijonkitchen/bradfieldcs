#include <assert.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define NOOP   0x00
#define LOADW  0x01
#define STOREW 0x02
#define ADD    0x03
#define SUB    0x04
#define ADDI   0x05
#define SUBI   0x06
#define JUMP   0x07
#define BEQZ   0x08
#define HALT   0xff

// TODO: highlight recent updates to non-PC registers
void print_registers(uint16_t *registers) {
	printw("\tPC: ");

	attron(A_BOLD);
	printw("0x%02x\n", registers[0]);
	attroff(A_BOLD);

	printw("\tR1: ");

	attron(A_BOLD);
	printw("%d\n", registers[1]);
	attroff(A_BOLD);

	printw("\tR2: ");

	attron(A_BOLD);
	printw("%d\n", registers[2]);
	attroff(A_BOLD);

	addch('\n');
}

int get_opsize(uint16_t pc, uint8_t *memory) {
	uint8_t op = memory[pc];
	int opsize = 3;
	if (op == HALT || op == NOOP) {
		opsize = 1;
	} else if (op == JUMP) {
		opsize = 2;
	}
	return opsize;
}

// TODO: highlight recent updates to memory
void print_memory(uint16_t pc, uint8_t *memory) {
	int opsize = get_opsize(pc, memory);

	// ASCII arrow to pc location
	addch('\t');
	for (int i = 0; i < 3 * pc; i++) {
		addch(' ');
	}
	printw("PC\n");
	addch('\t');
	for (int i = 0; i < 3 * pc + 1; i++) {
		addch(' ');
	}
	addch('v');
	addch('\n');

	// Memory
	addch('\t');
	for (int i = 0; i < 24; i++) {
		if (i > 0) {
			addch(' ');
		}
		printw("%02x", i);
	}
	addch('\n');
	addch('\t');
	for (int i = 0; i < 24; i++) {
		if (i > 0) {
			addch(' ');
		}
		addch(ACS_HLINE);
		addch(ACS_HLINE);
	}
	addch('\n');

	attron(A_BOLD);
	addch('\t');
	for (int i = 0; i < 24; i++) {
		if (i == pc) {
			if (i > 0) {
				addch(' ');
			}
			attron(A_STANDOUT);
		} else if (i == pc + opsize) {
			attroff(A_STANDOUT);
			addch(' ');
		} else if (i > 0) {
			addch(' ');
		}
		printw("%02x", memory[i]);
	}
	addch('\n');
	attroff(A_BOLD);

	printw("\tINSTRUCTIONS ");
	for (int i = 0; i < 39; i++) {
		addch(ACS_HLINE);
	}
	printw("^ OUT-^ IN-1^ IN-2^\n");
	addch('\n');
}

void print_instruction(uint16_t pc, uint8_t *memory) {
	uint8_t op = memory[pc];
	if (op == HALT) {
		printw("HALT");
	} else if (op == NOOP) {
		printw("NOOP");
	} else if (op == JUMP) {
		printw("JUMP\t0x%02x", memory[pc + 1]);
	} else {
		uint8_t arg1 = memory[pc + 1];
		uint8_t arg2 = memory[pc + 2];

		switch (op) {
			case LOADW:
				printw("LOADW\tR%d\t(0x%02x)", arg1, arg2);
				break;

			case STOREW:
				printw("STOREW\tR%d\t(0x%02x)", arg1, arg2);
				break;

			case ADD:
				printw("ADD\tR%d\tR%d", arg1, arg2);
				break;

			case SUB:
				printw("SUB\tR%d\tR%d", arg1, arg2);
				break;

			case ADDI:
				printw("ADDI\tR%d\t%d", arg1, arg2);
				break;

			case SUBI:
				printw("SUBI\tR%d\t%d", arg1, arg2);
				break;

			case BEQZ:
				printw("BEQZ\tR%d\t0x%02x", arg1, arg2);
				break;
		}
	}
	addch('\n');
}

void print_instructions(uint16_t pc, uint8_t *memory) {
	for (int i = 0; i < 18; i += get_opsize(i, memory)) {
		addch('\t');
		printw("0x%02x\t", i);

		attron(A_BOLD);
		if (i == pc) {
			attron(A_STANDOUT);
			print_instruction(i, memory);
			attroff(A_STANDOUT);
		} else {
			print_instruction(i, memory);
		}
		attroff(A_BOLD);
	}
}

void title(char *name, int size) {
	attron(A_STANDOUT);
	printw("%s", name);
	for (int i = strlen(name); i < size; i++) {
		addch(' ');
	}
	attroff(A_STANDOUT);
	printw("\n\n");
}

void print_state(uint16_t *registers, uint8_t *memory) {
	clear();
	move(0, 0);

	uint16_t pc = registers[0];

	title("REGISTERS", 80);
	print_registers(registers);

	title("MEMORY", 80);
	print_memory(pc, memory);

	title("PROGRAM", 80);
	print_instructions(pc, memory);

	refresh();
}

void run_vm(uint8_t *memory) {
	uint16_t registers[3] = {
		0x0000,
		0x0000,
		0x0000
	};

	while (1) {
		print_state(registers, memory);
		getch();

		uint16_t pc = registers[0];
		uint8_t instruction = memory[pc];

		// Handle HALT and NOOP separately because they don't have any args.
		if (instruction == HALT) {
			return;
		}

		if (instruction == NOOP) {
			registers[0]++;
			continue;
		}

		// Handle JUMP separately because it only has one arg.
		if (instruction == JUMP) {
			registers[0] = memory[pc + 1];
			continue;
		}

		uint8_t arg1 = memory[pc + 1];
		uint8_t arg2 = memory[pc + 2];

		uint8_t *byte_pointer;
		uint16_t *word_pointer;

		switch (instruction) {
			// load_word reg (addr)
			case LOADW:
				byte_pointer = memory + arg2;
				word_pointer = (uint16_t *)byte_pointer;

				registers[arg1] = *word_pointer;
				break;

			// store_word reg (addr)
			case STOREW:
				byte_pointer = memory + arg2;
				word_pointer = (uint16_t *)byte_pointer;

				*word_pointer = registers[arg1];
				break;

			// add reg1 reg2
			case ADD:
				registers[arg1] += registers[arg2];
				break;

			// sub reg1 reg2
			case SUB:
				registers[arg1] -= registers[arg2];
				break;

			case ADDI:
				registers[arg1] += arg2;
				break;

			case SUBI:
				registers[arg1] -= arg2;
				break;

			case BEQZ:
				if (registers[arg1] == 0) {
					registers[0] += arg2;
				}
				break;

			default:
				endwin();
				fprintf(stderr, "Unrecognized instruction: 0x%x\n", instruction);
				exit(1);
		}

		// All 2-arg instructions (even BEQZ) advance PC by the same amount.
		registers[0] += 3;
	}
}

int main() {
	uint8_t memory[24] = {
		0x01, 0x01, 0x14, // load_word r1 (0x14)
		0x08, 0x01, 0x08, // beqz r1 0x08
		0x03, 0x02, 0x01, // add r2 r1
		0x06, 0x01, 0x01, // subi r1 1
		0x07, 0x03,       // jump 3
		0x02, 0x02, 0x12, // store r2 (0x12)
		0xff,             // halt

		0x00, 0x00,       // output

		0x0a, 0x00,       // input 1
		0x00, 0x00        // input 2
	};

	initscr();
	cbreak();
	curs_set(0);
	noecho();
	run_vm(memory);
	endwin();
}
