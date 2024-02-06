
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream> 
#include <string> 


using namespace std;


int main(int argc, char* argv[]) 
{
    ifstream file;
    file.open(argv[1]);

    string i = argv[2];
    int iNum = stoi(i);


    string line;

    unsigned int memory[1000];
    unsigned int r[32] = {0};
    unsigned int counter = 0;
    unsigned int pc = 0;
    unsigned int instruction;
    



    while (getline(file, line))
    {  
        stringstream ss; 
        ss << line;

        ss >> hex >> instruction;

        memory[counter] = instruction;
        counter++;
    }


    while (iNum <= counter)
    {
        instruction =  memory[pc];
        unsigned int opcode = instruction >> 26;

        if(opcode == 0) //r-type
        {
            unsigned int rs = (instruction & 0x03FFFFFF) >> 21;
            unsigned int rt = (instruction & 0x001FFFFF) >> 16;
            unsigned int rd = (instruction & 0x0000FFFF) >> 11;
            unsigned int sa = (instruction & 0x000007FF) >> 6;
            unsigned int function =  (instruction & 0x0000003F);
            
            switch (function)
            {

            case 0x20: //add 
                r[rd] = (unsigned int)((int)r[rs] + (int)r[rt]);
                break;
            case 0x21: //addu
                r[rd] = r[rs] + r[rt];
                break;
            case 0x24: //and
                r[rd] = r[rs] & r[rt];
                break;
            case 0x25: //or
                r[rd] = r[rs] | r[rt];
                break; 
            case 0x27: //nor
                r[rd] = ~(r[rs] | r[rt]);
                break;
            case 0x2A: //slt
                if ((int)r[rs] < (int)r[rt])
                {
                    r[rd] = 1;
                }
                else
                {
                    r[rd] = 0;
                }
                break;
            case 0x2B: //sltu    
                if (r[rs] < r[rt])
                {
                    r[rd] = 1;
                }
                else
                {
                    r[rd] = 0;
                }
                break;
            case 0x00: //sll
                r[rd] = r[rt] << r[sa];
                break;
            case 0x02: //srl
                r[rd] = r[rt] >> r[sa];
                break;
            case 0x03: //sra
                r[rd] = (unsigned int)((int)r[rt] >> (int)r[sa]);
                break;
            case 0x22: //sub
                r[rd] = (unsigned int)((int)r[rs] - (int)r[rt]);
                break;
            case 0x23: //subu
                r[rd] = r[rs] - r[rt];
                break;
            case 0x08://jr
                pc  = r[rs];
                break;
            default:       
            }
            pc++;

        }
        else if(opcode == 0b100011 || opcode == 0b101011 || opcode == 0b100100 || opcode == 0b100101|| opcode == 0b101001 || opcode == 0b101000)
        {
            unsigned int rs = (instruction & 0x03FFFFFF) >> 21;
            unsigned int rt = (instruction & 0x001FFFFF) >> 16;
            unsigned int offset =  (instruction & 0x0000FFFF);

            switch(opcode)
            {
                case 0x04:// beq
                    if(r[rs] == r[rt])
                        {
                            pc = pc + offset * 4;
                        }
                    break; 
                case 0x05:// bne
                    if((int)r[rs] == (int)r[rt])
                        {
                            pc = pc + offset * 4;
                        }
                    break;
                case 0x23://lw
                    r[rt] = *(unsigned int*)((char*)&memory[0] + offset + r[rs]);
                    break;
                case 0x2B: //sw
                    *(unsigned int*)((char*)&memory[0] + offset + r[rs]) = r[rt];
                    break;
                case 0x25: //lhu
                    r[rt] = *(unsigned short*)((char*)&memory[0] + offset + r[rs]);
                    break;
                case 0x29: //sh
                    *(unsigned short*)((char*)&memory[0] + offset + r[rs]) = r[rt];
                    break;
                case 0x24: //lbu
                    r[rt] = *(unsigned char*)((char*)&memory[0] + offset + r[rs]);
                    break;
                case 0x28: //sb
                    *(unsigned char*)((char*)&memory[0] + offset + r[rs]) = r[rt];
                    break;
                default:
            }
            pc++;
        }
        else if(opcode == 0b000011 || opcode == 0b000010) //j-type
        {
            unsigned int target = (instruction & 0x03FFFFFF);

            switch(opcode)
            {
                case 0b000010: //j
                    pc = (pc  & 0xF0000000)| target << 2;
                    break;
                case 0b000011: //jal;
                    pc = target << 2;
                    break;
                default:
            }
            pc++;

        }
        else //i-type
        {
            unsigned int rs = (instruction & 0x03FFFFFF) >> 21;
            unsigned int rt = (instruction & 0x001FFFFF) >> 16;
            unsigned int im = (instruction & 0x0000FFFF); 
    
            switch(opcode)
            {
                case 0x08: //add
                    r[rt] = unsigned((int)r[rs] + (int)im); 
                case 0x09: //addiu
                    r[rt] = r[rs] + im;
                case 0x0C: //andi
                    r[rt] = r[rs] & im;
                case 0x0F: //lui
                    r[rt] = im << 16; 
                case 0x0D://ori
                    r[rt] = r[rs] | im;
                case 0x0A://slti
                    if ((int)r[rs] < (int)im)
                    {
                        r[rt] = 1;
                    }
                    else
                    {
                        r[rt] = 0;
                    }
                    break;
                case 0x0B://sltiu
                    if (r[rs] < im)
                    {
                        r[rt] = 1;
                    }
                    else
                    {
                        r[rt] = 0;
                    }
                    default:
            }
            pc++;     
        }
    }
}