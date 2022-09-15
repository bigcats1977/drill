
/* 程序的常用全局宏定义 */
#define WM_COLLECTTIMEROUT      WM_USER + 5001
#define WM_GUARDTIMEROUT        WM_USER + 5002
#define WM_GUARDREADTIMEROUT    WM_USER + 5003
#define WM_PORTBUFFTIMEROUT     WM_USER + 5004
#define WM_COLLECTSCMTIMEROUT   WM_USER + 5005
#define WM_SAVEDATATIMEROUT     WM_USER + 5006
#define WM_SAVEDEBUGTIMEROUT    WM_USER + 5007
#define WM_ALARMPLAYTIMEROUT    WM_USER + 5008
#define WM_READVALVETIMEROUT    WM_USER + 5009

#define WM_INTERPT_CHANGE       WM_USER + 6001
#define WM_UPDATE_SELPOS        WM_USER + 6002
#define WM_INTERPT_ZOOMIN       WM_USER + 6003
#define WM_COLLECT_DATA         WM_USER + 6004
#define WM_CALIB_DATA           WM_USER + 6005

#define CLR_ABNORMDATA      RGB(255,0,0)        // 异常数据颜色
#define CLR_TOOLBUCKLE      RGB(0,0,255)        // 工具扣数据颜色
#define CLR_ALTERBACKG      RGB(215,247,241)    // 隔行背景颜色

#define VALID_YEAR          5       // 软件有效期年份
/* 强制设置CTime的长度为4, VC2008认为长度为8，VC6认为为0，两者不一致 */
#define CTIMELEN            4

#define PORTBUFF            108
#define MAX_LOADSTRING      512

#define LANGUAGE_CHINESE    0
#define LANGUAGE_ENGLISH    1
//#define LANGUAGE_RUSSIAN    2
#define LANGUAGE_NUM        (LANGUAGE_ENGLISH+1)

#define BIGPOINT            _T("\xA1\xF1")
//#define BIGPOINT            "\x00\xB7"

/* Dialog kind */
#define DLG_COLLECTDATA     0
#define DLG_SEGCALIB        1

/*定时器ID定义*/
#define COOLCONTROL_TIMER   0       // COOL控件使用定时器
#define PORTREAD_TIMER      1       // 读取串口数据定时器
#define GUARD_TIMER         2       // 上扣后保护定时器
#define GUARDREAD_TIMER     4       // 上扣保护时读取串口数据的定时器
#define BEARSHOW_TIMER      5       // 显示肩负选择对话框定时器
#define BEARSHOW_LEN        5000    // 程序启动前选择肩负和大扭矩对话框等待时间
#define AUTOSAVE_TIMER      6       // 定时保存CRC错误和调试信息定时器
#define AUTOSAVE_TLEN       5000    // 定时保存CRC错误和调试信息定时时长
#define PORTBUFF_TIMER      7       // 485串口保护定时器，上位机发送命令前，如果串口有数据，定时保护
                                    // 单片机发送完成后，再发送命令
#define PORTBUFF_TLEN       15      // 串口发送12个BYTE需要13ms,定时器设置为15ms
#define COLLECT_TIMER       10      // 收集单片机数据定时器
#define COLLECT_TLEN        1000    // 收集单片机数据定时时长 //2000
#define WNDSHOW_TIMER       11      // 帮助旋转显示定时器
#define WNDSHOW_TLEN        100     // 帮助旋转显示定时时长
#define RESET_TIMER         12
#define ALARM_TIMER         13
#define ALRAM_DELAY         5000    /* 告警音播放时长5s */
#define READVALVE_TIMER     14      /* 定时读取阀门状态定时器，时长1s */
#define READVALVE_LEN       1000
#define SETCALIB_TIMER      15      /* 设置校准信息定时器 */
#define SETCALIB_LEN        400


#define VITTANAMENUM        50

#define SPRINTFLEN          60

/* 0: 真实串口；1：模拟测试；2：读取dat扭矩数据；3: 读取dat 历史数据*/
#define COLL_PORT           0
#define COLL_RAND           1
#define COLL_TORQUE         2
#define COLL_MULTITORQ      3
#define COLL_HISTORY        4

#define NULLSTR             _T("")
/* 1 N.m = 0.737463 lb.ft */
#define NM2LBFT             0.737463
#define LBFT2NM             1.35582

#ifndef WIDTHBYTES
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
#endif

#ifndef BFT_BITMAP
#define BFT_BITMAP          0x4d42   // 'BM'
#endif

/* 数据标准化，小数点后两位 */
#define PERCENTDATA(fData)    (int(fData*100)/100.0)

#define MIN(a, b)            ((a) > (b) ? (b) : (a))
#define MAX(a, b)            ((a) > (b) ? (a) : (b))

/* 3.22后所有串口数据都记录,数据包括脉冲信息，根据脉冲算周数 */
#define VERSION_RECPLUS(ptTorq)  (ptTorq->dwdelplus_size() > 0)

/* 自定义代码宏，降低代码圈复杂度 */
/* 程序公共宏值 */
/* 指针为空，直接返回 */
#define ASSERT_NULL(point)              {                   \
        if((point) == NULL)                                 \
            return;                                         \
    }

/* 指针为空，返回ret(BOOL, value, NULL) */
#define ASSERT_NULL_R(point, ret)       {                   \
        if((point) == NULL)                                 \
            return (ret);                                   \
    }

/* 参数为0，直接返回 */
#define ASSERT_ZERO(value)              {                   \
        if((value) == 0)                                    \
            return;                                         \
    }

/* 参数为0，返回ret(BOOL, value, NULL)  */
#define ASSERT_ZERO_R(value, ret)       {                   \
        if((value) == 0)                                    \
            return (ret);                                   \
    }

/* 参数为TRUE，直接返回 */
#define COMP_BTRUE(value)               {                   \
        if((value))                                         \
            return;                                         \
    }

/* 参数为FALSE，直接返回 */
#define COMP_BFALSE(value)              {                   \
        if(!(value))                                        \
            return;                                         \
    }

/* 参数为TRUE，返回 ret */
#define COMP_BTRUE_R(value, ret)        {                   \
        if((value))                                         \
            return (ret);                                   \
    }

/* 参数为FALSE，返回 ret */
#define COMP_BFALSE_R(value, ret)        {                  \
        if(!(value))                                        \
            return (ret);                                   \
    }

/* 参数等于cp，直接返回 */
#define COMP_BE(value, cp)              {                   \
        if((value) == (cp))                                 \
            return;                                         \
    }

/* 参数等于cp，返回ret(BOOL, value, NULL)  */
#define COMP_BE_R(value, cp, ret)       {                   \
        if((value) == (cp))                                 \
            return (ret);                                   \
    }

/* 参数不等于cp，直接返回 */
#define COMP_BNE(value, cp)             {                   \
        if((value) != (cp))                                 \
            return;                                         \
    }

/* 参数不等于cp，返回ret(BOOL, value, NULL)  */
#define COMP_BNE_R(value, cp, ret)      {                   \
        if((value) != (cp))                                 \
            return (ret);                                   \
    }

/* 参数小于cp，直接返回 */
#define COMP_BL(value, cp)              {                   \
        if((value) < (cp))                                  \
            return;                                         \
    }

/* 参数小于cp，返回ret(BOOL, value, NULL)  */
#define COMP_BL_R(value, cp, ret)       {                   \
        if((value) < (cp))                                  \
            return (ret);                                   \
    }

/* 参数大于cp，直接返回 */
#define COMP_BG(value, cp)              {                   \
        if((value) > (cp))                                  \
            return;                                         \
    }

/* 参数大于cp，返回ret(BOOL, value, NULL)  */
#define COMP_BG_R(value, cp, ret)       {                   \
        if((value) > (cp))                                  \
            return (ret);                                   \
    }

/* 参数小于等于cp，直接返回 */
#define COMP_BLE(value, cp)             {                   \
        if((value) <= (cp))                                 \
            return;                                         \
    }

/* 参数小于等于cp，返回ret(BOOL, value, NULL)  */
#define COMP_BLE_R(value, cp, ret)      {                   \
        if((value) <= (cp))                                 \
            return (ret);                                   \
    }

/* 参数大于等于cp，直接返回 */
#define COMP_BGE(value, cp)             {                   \
        if((value) >= (cp))                                 \
            return;                                         \
    }

/* 参数大于等于cp，返回ret(BOOL, value, NULL)  */
#define COMP_BGE_R(value, cp, ret)      {                   \
        if((value) >= (cp))                                 \
            return (ret);                                   \
    }


/* 参数为NULL，循环里面continue */
#define COMP_BNULL_CONTINUE(value)      {                   \
        if((value) == NULL)                                 \
            continue;                                       \
    }

/* 参数为TRUE，循环里面continue */
#define COMP_BTRUE_CONTINUE(value)      {                   \
        if((value))                                         \
            continue;                                       \
    }

/* 参数为FALSE，循环里面continue */
#define COMP_BFALSE_CONTINUE(value)     {                   \
        if(!(value))                                        \
            continue;                                       \
    }

/* 删除非空指针 */
#define DELETE_POINT(point)             {                   \
        if((point) != NULL)                                 \
        {                                                   \
            delete (point);                                 \
            (point) = NULL;                                 \
        }                                                   \
    }

/* 删除有效的object */
#define DELETE_OBJECT(hObject)          {                   \
        if(hObject)                                         \
            ::DeleteObject(hObject);                        \
    }

/* 参数上限检查，如果检查的值大于某值，将该值修改为某值 */
#define CHECK_VALUE_UP(check, upper)    {                   \
        if(check > upper)                                   \
        {                                                   \
            (check) = (upper);                              \
        }                                                   \
    }

/* 参数下限检查，如果检查的值小于某值，将该值修改为某值*/
#define CHECK_VALUE_LOW(check, lower)   {                   \
        if(check < lower)                                   \
        {                                                   \
            (check) = (lower);                              \
        }                                                   \
    }


/* 程序特有宏值 */
/* 判断扭矩记录的序号是否有效 */
#define JUDGE_RANGE(nNo, iMaxNo)        {                   \
        if(((nNo) < 1                      )  ||            \
           ((nNo) > (iMaxNo)               )  ||            \
           ((nNo) > MAXWELLNUM             ))               \
        {                                                   \
            return;                                         \
        }                                                   \
    }

#define JUDGE_RANGE_BOOL(nNo, iMaxNo)   {                   \
        if(((nNo) < 1                      )  ||            \
           ((nNo) > (iMaxNo)               )  ||            \
           ((nNo) > MAXWELLNUM             ))               \
        {                                                   \
            return FALSE;                                   \
        }                                                   \
    }

/* 判断扭矩记录的序号是否有效，无效打印信息 */
#define JUDGE_RANGE_MESS(nNo, iMaxNo, strInfo)  {           \
        if(((nNo) < 1                      )  ||            \
           ((nNo) > (iMaxNo)               )  ||            \
           ((nNo) > MAXWELLNUM             ))               \
        {                                                   \
            theApp.SaveShowMessage(strInfo);                \
            return;                                         \
        }                                                   \
    }

/* 读取扭矩结构历史文件越界保护 */
#define CHECK_FILE_LEN(file, iFileLen)  {                   \
        if((file).GetPosition() > (iFileLen))               \
        {                                                   \
            return FALSE;                                   \
        }                                                   \
    }

#define CHECK_PARA_CHANGE(dstPara, modPara, bChange)  {     \
        if((dstPara) != (modPara))                          \
        {                                                   \
            (dstPara) = (modPara);                          \
            (bChange) |= TRUE;                              \
        }                                                   \
    }
#if 0
/* 从第一条记录开始，跳到指定的记录位置
   说明:文件的第一个UINT记录总数已经读出
   for循环的递增为int i 
   nLeng 为UNIT类型,表示该数据完整长度 */
#define SEEK_TORQUE(index, nLeng)      {                                    \
        for(i=0; i<(index); i++)                                            \
        {                                                                   \
            file.Read(&(nLeng), sizeof(UINT));                              \
            file.Seek((nLeng), CFile::current);                             \
        }                                                                   \
    }
#endif
/* 获取图像的控制扭矩 */
#define GET_CTRL_TORQ(fTorq, ptTorq)        {                       \
        fTorq = ptTorq->fmaxtorq();                                 \
        if(0 == fTorq && ptTorq->ftorque_size() > 1)                \
            fTorq = ptTorq->ftorque(ptTorq->ftorque_size()-1);      \
    }

/* 判断程序注册状态 */
#define JUDGE_REG_STATUS()              {                       \
        if(!theApp.m_tdbReg.bReged)                             \
        {                                                       \
            AfxMessageBox(IDS_STRINFNOREG, MB_ICONINFORMATION); \
            return;                                             \
        }                                                       \
    }

/* 设置控件的名称为STRINGTABEL指定的字符串 */
#define SET_CONTROL_NAME(CtrlID, strTabID, strTag)                          \
    {                                                                       \
        strTag.LoadString(strTabID);                                        \
        GetDlgItem(CtrlID)->SetWindowText(strTag);                          \
    }

/* 设置质量位属性，满足条件设置为1，否则清0 */
#define SET_QUALITY_BIT(tCondition, bitFlag, dwQua)     {                   \
    if(tCondition)                                                          \
    {                                                                       \
        dwQua |= bitFlag;                                                   \
        return dwQua;                                                       \
    }                                                                       \
    else                                                                    \
        dwQua &= ~bitFlag;                                                  \
}

#define HUNDREDTH(fNum)     (((UINT)((fNum) * 100)) / 100.0)
#define THOUSANDTH(fNum)    (((UINT)((fNum) * 1000)) / 1000.0)
#define HAND_CEIL(fNum)     (ceil((fNum)/100) * 100)
#define HAND_FLOOR(fNum)    (floor((fNum)/100) * 100)
#define GetRandom( min, max ) ((rand() % (int)(((max)+1) - (min))) + (min))

/* 显示周数信息，如果nCur=nAll，显示周数；否则显示当前周数/总周数 */
#define SHOWCIRINFO(nCur, nAll, fMaxCir, strCir)                                \
    {                                                                           \
        if(nCur == nAll)                                                        \
            strCir.Format("%.3f", (nAll*fMaxCir/MAXLINEITEM));                  \
        else                                                                    \
            strCir.Format("%.3f / %.3f", (nCur*fMaxCir/MAXLINEITEM),            \
                                        (nAll*fMaxCir/MAXLINEITEM));            \
    }


#define JUDGE_STRPARA_CHANGE(str1,str2)     {                               \
        clrCtrl = m_clrNormal;                                              \
        if(str1.Compare(str2) != 0)                                         \
            clrCtrl = m_clrChanged;                                         \
        pDC->SetTextColor(clrCtrl);                                         \
    }
#define JUDGE_NUMBERPARA_CHANGE(val1,val2)  {                               \
        clrCtrl = m_clrNormal;                                              \
        if(val1 != val2)                                                    \
            clrCtrl = m_clrChanged;                                         \
        pDC->SetTextColor(clrCtrl);                                         \
    }
#define JUDGE_OTHERTUBING_CHANGE(strOrg)    {                               \
        if (theApp.m_ptCurShow->tTubingCfg.bFixed == 0)      {              \
            JUDGE_NUMBERPARA_CHANGE(0, 1);          }                       \
        else                                        {                       \
            JUDGE_STRPARA_CHANGE(strContent,strOrg);}                       \
    }