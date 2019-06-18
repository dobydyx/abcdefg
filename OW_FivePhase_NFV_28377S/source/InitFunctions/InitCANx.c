/*
 * InitCANx.c
 *
 *  Created on: 2018年5月7日
 *      Author: Ezreal
 */

/************************************************************
Description:初始化CANa,CANb
************************************************************/

#include "F28x_Project.h"
#include "CtrlParams.h"

void InitCana(void)
{
    InitCanaGpio();

    EALLOW;

    // Place CAN controller in init state, regardless of previous state.
    // This will put controller in idle, and allow the message object RAM to be programmed.
    CanaRegs.CAN_CTL.bit.Init = 1;
    CanaRegs.CAN_CTL.bit.SWR = 1;

//    CanaRegs.CAN_CTL.bit.PMD = 0;           //奇偶校验功能开启

    // Wait for busy bit to clear
    while(CanaRegs.CAN_IF1CMD.bit.Busy)
    {
    }

    // Clear the message value bit in the arbitration register.
    // This indicates the message is not valid and is a "safe" condition to leave the message object.
    // The same arb reg is used to program all the message objects.
    CanaRegs.CAN_IF1CMD.bit.DIR = 1;
    CanaRegs.CAN_IF1CMD.bit.Arb = 1;
    CanaRegs.CAN_IF1CMD.bit.Control = 1;
    CanaRegs.CAN_IF1ARB.all = 0;
    CanaRegs.CAN_IF1MCTL.all = 0;

    CanaRegs.CAN_IF2CMD.bit.DIR = 1;
    CanaRegs.CAN_IF2CMD.bit.Arb = 1;
    CanaRegs.CAN_IF2CMD.bit.Control = 1;
    CanaRegs.CAN_IF2ARB.all = 0;
    CanaRegs.CAN_IF2MCTL.all = 0;

    // Loop through to program all 32 message objects
    int16_t iMsg;
    for(iMsg = 1; iMsg <= 32; iMsg+=2)
    {
        // Wait for busy bit to clear
        while(CanaRegs.CAN_IF1CMD.bit.Busy)
        {
        }

        // Initiate programming the message object
        CanaRegs.CAN_IF1CMD.bit.MSG_NUM = iMsg;

        // Wait for busy bit to clear
        while(CanaRegs.CAN_IF2CMD.bit.Busy)
        {
        }

        // Initiate programming the message object
        CanaRegs.CAN_IF2CMD.bit.MSG_NUM = iMsg + 1;
    }

    // Make sure that the interrupt and new data flags are updated for the message objects.
    CanaRegs.CAN_IF1CMD.bit.TXRQST = 1;
    CanaRegs.CAN_IF1CMD.bit.ClrIntPnd = 1;

    CanaRegs.CAN_IF2CMD.bit.TxRqst = 1;
    CanaRegs.CAN_IF2CMD.bit.ClrIntPnd = 1;

    // Loop through to program all 32 message objects
    for(iMsg = 1; iMsg <= 32; iMsg+=2)
    {
        // Wait for busy bit to clear
        while(CanaRegs.CAN_IF1CMD.bit.Busy)
        {
        }

        // Initiate programming the message object
        CanaRegs.CAN_IF1CMD.bit.MSG_NUM = iMsg;

        // Wait for busy bit to clear
        while(CanaRegs.CAN_IF2CMD.bit.Busy)
        {
        }

        // Initiate programming the message object
        CanaRegs.CAN_IF2CMD.bit.MSG_NUM = iMsg + 1;
    }

    // Acknowledge any pending status interrupts.
    volatile uint32_t discardRead = CanaRegs.CAN_ES.all;

    // Select CAN peripheral clock source
    ClkCfgRegs.CLKSRCCTL2.bit.CANABCLKSEL = 0;  //系统时钟

    // Set up the bit rate for the CAN bus.
    CanaRegs.CAN_CTL.bit.CCE = 1;           //Enabled configuration change bit
    CanaRegs.CAN_BTR.all = 0x00000727;      //tq = 0.2us; bit time = 2us; bit rate = 500kHz
    CanaRegs.CAN_CTL.bit.CCE = 0;           //Disabled configuration change bit

    //test mode
//    CanaRegs.CAN_CTL.bit.Test = 1;
//    CanaRegs.CAN_TEST.bit.EXL = 1;

    CanaRegs.CAN_CTL.bit.DAR = 1;           //关闭错误自动重发

    CanaRegs.CAN_CTL.bit.Init = 0;          //Place CAN controller out init state

    EDIS;
}

void CanaMessageSet(int ObjID, CanMsgObj *pMsgObj, MsgObjType eMsgType)
{
    EALLOW;

    uint32_t CmdMaskReg;
    uint32_t MaskReg;
    uint32_t ArbReg;
    uint32_t MsgCtrl;
    int bTransferData;
    int bUseExtendedID;

    bTransferData = 0;

    // Wait for busy bit to clear
    while(CanaRegs.CAN_IF1CMD.bit.Busy)
    {
    }

    // See if we need to use an extended identifier or not.
    if((pMsgObj->MsgID > 0x7ff) || (pMsgObj->Flags & 0x04))
    {
        bUseExtendedID = 1;
    }
    else
    {
        bUseExtendedID = 0;
    }

    // This is always a write to the Message object as this call is setting a
    // message object.  This call will also always set all size bits so it sets
    // both data bits.  The call will use the CONTROL register to set control
    // bits so this bit needs to be set as well.
    CmdMaskReg = 0x930000;

    // Initialize the values to a known state before filling them in based on
    // the type of message object that is being configured.
    ArbReg = 0;
    MsgCtrl = 0;
    MaskReg = 0;

    switch(eMsgType)
    {
        // Transmit message object.
        case Tx:
        {
            // Set the TXRQST bit and the reset the rest of the register.
            MsgCtrl |= 0x100;
            ArbReg = 0x20000000;
            bTransferData = 1;
            break;
        }

        // Transmit remote request message object
        case TxRemote:
        {
            // Set the TXRQST bit and the reset the rest of the register.
            MsgCtrl |= 0x100;
            ArbReg = 0;
            break;
        }

        // Receive message object.
        case Rx:
        {
            // This clears the DIR bit along with everything else.  The TXRQST
            // bit was cleared by defaulting MsgCtrl to 0.
            ArbReg = 0;
            break;
        }

        // Receive remote request message object.
        case RxRemote:
        {
            // The DIR bit is set to one for remote receivers.  The TXRQST bit
            // was cleared by defaulting MsgCtrl to 0.
            ArbReg = 0x20000000;

            // Set this object so that it only indicates that a remote frame
            // was received and allow for software to handle it by sending back
            // a data frame.
            MsgCtrl = 0x1000;

            // Use the full Identifier by default.
            MaskReg = 0x1FFFFFFF;

            // Make sure to send the mask to the message object.
            CmdMaskReg |= 0x400000;
            break;
        }

        // Remote frame receive remote, with auto-transmit message object.
        case RxTxRemote:
        {
            // Oddly the DIR bit is set to one for remote receivers.
            ArbReg = 0x20000000;

            // Set this object to auto answer if a matching identifier is seen.
            MsgCtrl = 0x1200;

            // The data to be returned needs to be filled in.
            bTransferData = 1;
            break;
        }
    }

    // Configure the Mask Registers.
    if(pMsgObj->Flags & 0x8)
    {
        if(bUseExtendedID)
        {
            // Set the 29 bits of Identifier mask that were requested.
            MaskReg = pMsgObj->MsgIDMask & 0x1FFFFFFF;
        }
        else
        {

            // Put the 11 bit Mask Identifier into the upper bits of the field
            // in the register.
            MaskReg = ((pMsgObj->MsgIDMask << 18) & 0x1FFC0000);
        }
    }

    // If the caller wants to filter on the extended ID bit then set it.
    if((pMsgObj->Flags & 0x28) == 0x28)
    {
        MaskReg |= 0x80000000;
    }

    // The caller wants to filter on the message direction field.
    if((pMsgObj->Flags & 0x18) == 0x18)
    {
        MaskReg |= 0x40000000;
    }

    if(pMsgObj->Flags & (0x00000008 | 0x00000018 | 0x00000028))
    {
        // Set the UMASK bit to enable using the mask register.
        MsgCtrl |= 0x1000;

        // Set the MASK bit so that this gets transferred to the Message
        // Object.
        CmdMaskReg |= 0x400000;
    }

    // Set the Arb bit so that this gets transferred to the Message object.
    CmdMaskReg |= 0x200000;

    // Configure the Arbitration registers.
    if(bUseExtendedID)
    {
        // Set the 29 bit version of the Identifier for this message object.
        // Mark the message as valid and set the extended ID bit.
        ArbReg |= (pMsgObj->MsgID & 0x1FFFFFFF) | 0x80000000 | 0x40000000;
    }
    else
    {
        // Set the 11 bit version of the Identifier for this message object.
        // The lower 18 bits are set to zero.
        // Mark the message as valid.
        ArbReg |= ((pMsgObj->MsgID << 18) & 0x1FFC0000) | 0x80000000;
    }

    // Set the data length since this is set for all transfers.  This is also a
    // single transfer and not a FIFO transfer so set EOB bit.
    MsgCtrl |= (pMsgObj->MsgLen & 0xF);

    // Mark this as the last entry if this is not the last entry in a FIFO.
    if((pMsgObj->Flags & 0x00000200) == 0)
    {
        MsgCtrl |= 0x80;
    }

    // Enable transmit interrupts if they should be enabled.
    if(pMsgObj->Flags & 0x00000001)
    {
        MsgCtrl |= 0x800;
    }

    // Enable receive interrupts if they should be enabled.
    if(pMsgObj->Flags & 0x00000002)
    {
        MsgCtrl |= 0x400;
    }

    // Write the data out to the CAN Data registers if needed.
    if(bTransferData)
    {
//        CANDataRegWrite(pMsgObj->pMsgData,
//                        (uint32_t *)(0x00048000 + 0x110),
//                        pMsgObj->MsgLen);
        CanaRegs.CAN_IF1DATA.all = 0xAAAAAAAA;
        CanaRegs.CAN_IF1DATB.all = 0x55555555;

    }

    // Write out the registers to program the message object.
//    HWREGH(ui32Base + CAN_O_IF1CMD + 2) = CmdMaskReg >> 16;
    CanaRegs.CAN_IF1CMD.all = CmdMaskReg;

//    HWREGH(ui32Base + CAN_O_IF1MSK) = ui32MaskReg & CAN_REG_WORD_MASK;
//    HWREGH(ui32Base + CAN_O_IF1MSK + 2) = ui32MaskReg >> 16;
    CanaRegs.CAN_IF1MSK.all = MaskReg;

//    HWREGH(ui32Base + CAN_O_IF1ARB) = ui32ArbReg & CAN_REG_WORD_MASK;
//    HWREGH(ui32Base + CAN_O_IF1ARB + 2) = ui32ArbReg >> 16;
    CanaRegs.CAN_IF1ARB.all = ArbReg;

//    HWREGH(ui32Base + CAN_O_IF1MCTL) = ui32MsgCtrl & CAN_REG_WORD_MASK;
    CanaRegs.CAN_IF1MCTL.all = MsgCtrl;

    // Transfer the message object to the message object specific by ui32ObjID.
//    HWREGH(ui32Base + CAN_O_IF1CMD) = ui32ObjID & CAN_IF1CMD_MSG_NUM_M;
    CanaRegs.CAN_IF1CMD.bit.MSG_NUM = ObjID;
    EDIS;
    return;
}
