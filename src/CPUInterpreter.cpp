#include "CPUInterpreter.h"
#include "MemoryManager.h"

CPUInterpreter& CPUInterpreter::GetInstance() { static CPUInterpreter i; return i; }

bool CPUInterpreter::Reset() {
    m_registers.PC = 0x80000000;
    m_registers.SP = 0x00000000;
    m_registers.LR = 0x00000000;
    for (int i = 0; i < 31; i++) m_registers.X[i] = 0;
    m_registers.FLAGS = 0;
    m_cycleCount = 0;
    return true;
}

void CPUInterpreter::ExecuteFrame(float deltaSeconds) {
    int targetCycles = (int)(1785000.0f * deltaSeconds);
    int cyclesExecuted = 0;

    while (cyclesExecuted < targetCycles) {
        uint32_t instruction = FetchInstruction();
        int cycles = ExecuteInstruction(instruction);
        cyclesExecuted += cycles;
        m_cycleCount += cycles;
    }
}

uint32_t CPUInterpreter::FetchInstruction() {
    uint32_t instr = MemoryManager::GetInstance().Read32(m_registers.PC);
    m_registers.PC += 4;
    return instr;
}

int CPUInterpreter::ExecuteInstruction(uint32_t instr) {
    uint8_t opcode = (instr >> 24) & 0xFF;

    switch (opcode) {
        case 0x00: return 4; // NOP
        case 0x01: return ExecuteArithmetic(instr);
        case 0x02: return ExecuteMemory(instr);
        case 0x03: return ExecuteBranch(instr);
        case 0x04: return ExecuteSystem(instr);
        default: return 4;
    }
}

int CPUInterpreter::ExecuteArithmetic(uint32_t instr) {
    uint8_t op = (instr >> 20) & 0xF;
    uint8_t rd = (instr >> 16) & 0xF;
    uint8_t rn = (instr >> 12) & 0xF;
    uint8_t rm = (instr >> 8) & 0xF;
    uint8_t shift = (instr >> 4) & 0xF;

    uint64_t operand2 = m_registers.X[rm];
    switch (shift) {
        case 0: break;
        case 1: operand2 <<= ((instr >> 2) & 0x3F); break;
        case 2: operand2 >>= ((instr >> 2) & 0x3F); break;
        case 3: operand2 = (int64_t)operand2 >> ((instr >> 2) & 0x3F); break;
    }

    switch (op) {
        case 0: m_registers.X[rd] = m_registers.X[rn] + operand2; break;
        case 1: m_registers.X[rd] = m_registers.X[rn] - operand2; break;
        case 2: m_registers.X[rd] = m_registers.X[rn] & operand2; break;
        case 3: m_registers.X[rd] = m_registers.X[rn] | operand2; break;
        case 4: m_registers.X[rd] = m_registers.X[rn] ^ operand2; break;
        case 5: m_registers.X[rd] = m_registers.X[rn] * operand2; break;
        case 6: if (operand2 != 0) m_registers.X[rd] = m_registers.X[rn] / operand2; break;
    }

    return 1;
}

int CPUInterpreter::ExecuteMemory(uint32_t instr) {
    uint8_t op = (instr >> 20) & 0xF;
    uint8_t rd = (instr >> 16) & 0xF;
    uint8_t rn = (instr >> 12) & 0xF;
    int16_t offset = instr & 0xFFFF;

    uint64_t addr = m_registers.X[rn] + offset;

    switch (op) {
        case 0: m_registers.X[rd] = MemoryManager::GetInstance().Read8(addr); break;
        case 1: m_registers.X[rd] = MemoryManager::GetInstance().Read16(addr); break;
        case 2: m_registers.X[rd] = MemoryManager::GetInstance().Read32(addr); break;
        case 3: m_registers.X[rd] = MemoryManager::GetInstance().Read64(addr); break;
        case 4: MemoryManager::GetInstance().Write8(addr, (uint8_t)m_registers.X[rd]); break;
        case 5: MemoryManager::GetInstance().Write16(addr, (uint16_t)m_registers.X[rd]); break;
        case 6: MemoryManager::GetInstance().Write32(addr, (uint32_t)m_registers.X[rd]); break;
        case 7: MemoryManager::GetInstance().Write64(addr, m_registers.X[rd]); break;
    }

    return 2;
}

int CPUInterpreter::ExecuteBranch(uint32_t instr) {
    uint8_t op = (instr >> 20) & 0xF;
    int32_t offset = (int32_t)(instr & 0xFFFFFF) << 8 >> 8;

    switch (op) {
        case 0: m_registers.PC += offset * 4; break;
        case 1: m_registers.LR = m_registers.PC; m_registers.PC += offset * 4; break;
        case 2: if (m_registers.FLAGS & 0x1) m_registers.PC += offset * 4; break;
        case 3: if (!(m_registers.FLAGS & 0x1)) m_registers.PC += offset * 4; break;
    }

    return 3;
}

int CPUInterpreter::ExecuteSystem(uint32_t instr) {
    uint8_t op = (instr >> 20) & 0xF;

    switch (op) {
        case 0: return 4; // Halt
        case 1: return 4; // Wait for interrupt
        case 2: return 4; // Supervisor call
    }

    return 4;
}
