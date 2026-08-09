#pragma once
#include <cstdint>

struct CPURegisters {
    uint64_t X[31];
    uint64_t SP;
    uint64_t PC;
    uint64_t LR;
    uint32_t FLAGS;
};

class CPUInterpreter {
private:
    CPURegisters m_registers{};
    uint64_t m_cycleCount = 0;
    CPUInterpreter() = default;
    uint32_t FetchInstruction();
    int ExecuteInstruction(uint32_t instr);
    int ExecuteArithmetic(uint32_t instr);
    int ExecuteMemory(uint32_t instr);
    int ExecuteBranch(uint32_t instr);
    int ExecuteSystem(uint32_t instr);
public:
    static CPUInterpreter& GetInstance();
    CPUInterpreter* Allocate() { return this; }
    bool Reset();
    void ExecuteFrame(float deltaSeconds);
    const CPURegisters& GetRegisters() const { return m_registers; }
    uint64_t GetCycleCount() const { return m_cycleCount; }
};
