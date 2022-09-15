
/* ����ĳ���ȫ�ֺ궨�� */
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

#define CLR_ABNORMDATA      RGB(255,0,0)        // �쳣������ɫ
#define CLR_TOOLBUCKLE      RGB(0,0,255)        // ���߿�������ɫ
#define CLR_ALTERBACKG      RGB(215,247,241)    // ���б�����ɫ

#define VALID_YEAR          5       // �����Ч�����
/* ǿ������CTime�ĳ���Ϊ4, VC2008��Ϊ����Ϊ8��VC6��ΪΪ0�����߲�һ�� */
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

/*��ʱ��ID����*/
#define COOLCONTROL_TIMER   0       // COOL�ؼ�ʹ�ö�ʱ��
#define PORTREAD_TIMER      1       // ��ȡ�������ݶ�ʱ��
#define GUARD_TIMER         2       // �Ͽۺ󱣻���ʱ��
#define GUARDREAD_TIMER     4       // �Ͽ۱���ʱ��ȡ�������ݵĶ�ʱ��
#define BEARSHOW_TIMER      5       // ��ʾ�縺ѡ��Ի���ʱ��
#define BEARSHOW_LEN        5000    // ��������ǰѡ��縺�ʹ�Ť�ضԻ���ȴ�ʱ��
#define AUTOSAVE_TIMER      6       // ��ʱ����CRC����͵�����Ϣ��ʱ��
#define AUTOSAVE_TLEN       5000    // ��ʱ����CRC����͵�����Ϣ��ʱʱ��
#define PORTBUFF_TIMER      7       // 485���ڱ�����ʱ������λ����������ǰ��������������ݣ���ʱ����
                                    // ��Ƭ��������ɺ��ٷ�������
#define PORTBUFF_TLEN       15      // ���ڷ���12��BYTE��Ҫ13ms,��ʱ������Ϊ15ms
#define COLLECT_TIMER       10      // �ռ���Ƭ�����ݶ�ʱ��
#define COLLECT_TLEN        1000    // �ռ���Ƭ�����ݶ�ʱʱ�� //2000
#define WNDSHOW_TIMER       11      // ������ת��ʾ��ʱ��
#define WNDSHOW_TLEN        100     // ������ת��ʾ��ʱʱ��
#define RESET_TIMER         12
#define ALARM_TIMER         13
#define ALRAM_DELAY         5000    /* �澯������ʱ��5s */
#define READVALVE_TIMER     14      /* ��ʱ��ȡ����״̬��ʱ����ʱ��1s */
#define READVALVE_LEN       1000
#define SETCALIB_TIMER      15      /* ����У׼��Ϣ��ʱ�� */
#define SETCALIB_LEN        400


#define VITTANAMENUM        50

#define SPRINTFLEN          60

/* 0: ��ʵ���ڣ�1��ģ����ԣ�2����ȡdatŤ�����ݣ�3: ��ȡdat ��ʷ����*/
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

/* ���ݱ�׼����С�������λ */
#define PERCENTDATA(fData)    (int(fData*100)/100.0)

#define MIN(a, b)            ((a) > (b) ? (b) : (a))
#define MAX(a, b)            ((a) > (b) ? (a) : (b))

/* 3.22�����д������ݶ���¼,���ݰ���������Ϣ���������������� */
#define VERSION_RECPLUS(ptTorq)  (ptTorq->dwdelplus_size() > 0)

/* �Զ������꣬���ʹ���Ȧ���Ӷ� */
/* ���򹫹���ֵ */
/* ָ��Ϊ�գ�ֱ�ӷ��� */
#define ASSERT_NULL(point)              {                   \
        if((point) == NULL)                                 \
            return;                                         \
    }

/* ָ��Ϊ�գ�����ret(BOOL, value, NULL) */
#define ASSERT_NULL_R(point, ret)       {                   \
        if((point) == NULL)                                 \
            return (ret);                                   \
    }

/* ����Ϊ0��ֱ�ӷ��� */
#define ASSERT_ZERO(value)              {                   \
        if((value) == 0)                                    \
            return;                                         \
    }

/* ����Ϊ0������ret(BOOL, value, NULL)  */
#define ASSERT_ZERO_R(value, ret)       {                   \
        if((value) == 0)                                    \
            return (ret);                                   \
    }

/* ����ΪTRUE��ֱ�ӷ��� */
#define COMP_BTRUE(value)               {                   \
        if((value))                                         \
            return;                                         \
    }

/* ����ΪFALSE��ֱ�ӷ��� */
#define COMP_BFALSE(value)              {                   \
        if(!(value))                                        \
            return;                                         \
    }

/* ����ΪTRUE������ ret */
#define COMP_BTRUE_R(value, ret)        {                   \
        if((value))                                         \
            return (ret);                                   \
    }

/* ����ΪFALSE������ ret */
#define COMP_BFALSE_R(value, ret)        {                  \
        if(!(value))                                        \
            return (ret);                                   \
    }

/* ��������cp��ֱ�ӷ��� */
#define COMP_BE(value, cp)              {                   \
        if((value) == (cp))                                 \
            return;                                         \
    }

/* ��������cp������ret(BOOL, value, NULL)  */
#define COMP_BE_R(value, cp, ret)       {                   \
        if((value) == (cp))                                 \
            return (ret);                                   \
    }

/* ����������cp��ֱ�ӷ��� */
#define COMP_BNE(value, cp)             {                   \
        if((value) != (cp))                                 \
            return;                                         \
    }

/* ����������cp������ret(BOOL, value, NULL)  */
#define COMP_BNE_R(value, cp, ret)      {                   \
        if((value) != (cp))                                 \
            return (ret);                                   \
    }

/* ����С��cp��ֱ�ӷ��� */
#define COMP_BL(value, cp)              {                   \
        if((value) < (cp))                                  \
            return;                                         \
    }

/* ����С��cp������ret(BOOL, value, NULL)  */
#define COMP_BL_R(value, cp, ret)       {                   \
        if((value) < (cp))                                  \
            return (ret);                                   \
    }

/* ��������cp��ֱ�ӷ��� */
#define COMP_BG(value, cp)              {                   \
        if((value) > (cp))                                  \
            return;                                         \
    }

/* ��������cp������ret(BOOL, value, NULL)  */
#define COMP_BG_R(value, cp, ret)       {                   \
        if((value) > (cp))                                  \
            return (ret);                                   \
    }

/* ����С�ڵ���cp��ֱ�ӷ��� */
#define COMP_BLE(value, cp)             {                   \
        if((value) <= (cp))                                 \
            return;                                         \
    }

/* ����С�ڵ���cp������ret(BOOL, value, NULL)  */
#define COMP_BLE_R(value, cp, ret)      {                   \
        if((value) <= (cp))                                 \
            return (ret);                                   \
    }

/* �������ڵ���cp��ֱ�ӷ��� */
#define COMP_BGE(value, cp)             {                   \
        if((value) >= (cp))                                 \
            return;                                         \
    }

/* �������ڵ���cp������ret(BOOL, value, NULL)  */
#define COMP_BGE_R(value, cp, ret)      {                   \
        if((value) >= (cp))                                 \
            return (ret);                                   \
    }


/* ����ΪNULL��ѭ������continue */
#define COMP_BNULL_CONTINUE(value)      {                   \
        if((value) == NULL)                                 \
            continue;                                       \
    }

/* ����ΪTRUE��ѭ������continue */
#define COMP_BTRUE_CONTINUE(value)      {                   \
        if((value))                                         \
            continue;                                       \
    }

/* ����ΪFALSE��ѭ������continue */
#define COMP_BFALSE_CONTINUE(value)     {                   \
        if(!(value))                                        \
            continue;                                       \
    }

/* ɾ���ǿ�ָ�� */
#define DELETE_POINT(point)             {                   \
        if((point) != NULL)                                 \
        {                                                   \
            delete (point);                                 \
            (point) = NULL;                                 \
        }                                                   \
    }

/* ɾ����Ч��object */
#define DELETE_OBJECT(hObject)          {                   \
        if(hObject)                                         \
            ::DeleteObject(hObject);                        \
    }

/* �������޼�飬�������ֵ����ĳֵ������ֵ�޸�Ϊĳֵ */
#define CHECK_VALUE_UP(check, upper)    {                   \
        if(check > upper)                                   \
        {                                                   \
            (check) = (upper);                              \
        }                                                   \
    }

/* �������޼�飬�������ֵС��ĳֵ������ֵ�޸�Ϊĳֵ*/
#define CHECK_VALUE_LOW(check, lower)   {                   \
        if(check < lower)                                   \
        {                                                   \
            (check) = (lower);                              \
        }                                                   \
    }


/* �������к�ֵ */
/* �ж�Ť�ؼ�¼������Ƿ���Ч */
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

/* �ж�Ť�ؼ�¼������Ƿ���Ч����Ч��ӡ��Ϣ */
#define JUDGE_RANGE_MESS(nNo, iMaxNo, strInfo)  {           \
        if(((nNo) < 1                      )  ||            \
           ((nNo) > (iMaxNo)               )  ||            \
           ((nNo) > MAXWELLNUM             ))               \
        {                                                   \
            theApp.SaveShowMessage(strInfo);                \
            return;                                         \
        }                                                   \
    }

/* ��ȡŤ�ؽṹ��ʷ�ļ�Խ�籣�� */
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
/* �ӵ�һ����¼��ʼ������ָ���ļ�¼λ��
   ˵��:�ļ��ĵ�һ��UINT��¼�����Ѿ�����
   forѭ���ĵ���Ϊint i 
   nLeng ΪUNIT����,��ʾ�������������� */
#define SEEK_TORQUE(index, nLeng)      {                                    \
        for(i=0; i<(index); i++)                                            \
        {                                                                   \
            file.Read(&(nLeng), sizeof(UINT));                              \
            file.Seek((nLeng), CFile::current);                             \
        }                                                                   \
    }
#endif
/* ��ȡͼ��Ŀ���Ť�� */
#define GET_CTRL_TORQ(fTorq, ptTorq)        {                       \
        fTorq = ptTorq->fmaxtorq();                                 \
        if(0 == fTorq && ptTorq->ftorque_size() > 1)                \
            fTorq = ptTorq->ftorque(ptTorq->ftorque_size()-1);      \
    }

/* �жϳ���ע��״̬ */
#define JUDGE_REG_STATUS()              {                       \
        if(!theApp.m_tdbReg.bReged)                             \
        {                                                       \
            AfxMessageBox(IDS_STRINFNOREG, MB_ICONINFORMATION); \
            return;                                             \
        }                                                       \
    }

/* ���ÿؼ�������ΪSTRINGTABELָ�����ַ��� */
#define SET_CONTROL_NAME(CtrlID, strTabID, strTag)                          \
    {                                                                       \
        strTag.LoadString(strTabID);                                        \
        GetDlgItem(CtrlID)->SetWindowText(strTag);                          \
    }

/* ��������λ���ԣ�������������Ϊ1��������0 */
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

/* ��ʾ������Ϣ�����nCur=nAll����ʾ������������ʾ��ǰ����/������ */
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