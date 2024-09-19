// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
	BitLen = len;
	MemLen = (BitLen + (sizeof(TELEM) * 8 - 1)) / (sizeof(TELEM) * 8);
	pMem = new TELEM[MemLen];
	std::memset(pMem, 0, MemLen * sizeof(TELEM));
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
}

TBitField::~TBitField()//деструктор
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || (n > BitLen - 1))throw - 1;
	int res = n / 32;
	return res;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0 || (n > BitLen - 1))throw - 1;
	TELEM mask = 1;
	return mask << (n % 32);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	int i = GetMemIndex(n);
	TELEM t = GetMemMask(n);
	pMem[i] = pMem[i] | t;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || (n > BitLen - 1))throw - 1;
	int i = GetMemIndex(n);
	TELEM t = ~GetMemMask(n);
	pMem[i] &= t;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (this== &bf) {
		delete[] pMem;
		pMem = new TELEM[bf.MemLen];
		MemLen = bf.BitLen;
		for (int i = 0; i < MemLen; i++) {
			pMem[i] = bf.pMem[i];
		}
	}
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение,подумать как написать правильнее
{
	//либо от (MemLen-1)*sizeof(TELEM)*8 до BitLen;
	if (BitLen != bf.BitLen) {
		return 0;
	}
	for (int i = 0; i < MemLen; i++) {
		if (this->GetBit(i) != bf.GetBit(i))return 0;
	}

	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	int r = (bf == *this);
	if (r == 0)return 1;
	else return 0;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	int len, ump = BitLen, i = 0;
	if (BitLen >= bf.BitLen) {
		len = BitLen;
		ump = bf.BitLen;
	}
	else {
		len = bf.BitLen;
	}
	TBitField res(len);
	for (i = 0; i < ump; i++) {
		res.pMem[i] = bf.pMem[i] | pMem[i];
	}
	while (i < len) {
		if (BitLen > bf.BitLen) {
			res.pMem[i] = pMem[i];
		}
		else {
			res.pMem[i] = bf.pMem[i];
		}
		i++;
	}
	return res;

}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	int len, ump = BitLen, i = 0;
	if (BitLen >= bf.BitLen) {
		len = BitLen;
		ump = bf.BitLen;
	}
	else {
		len = bf.BitLen;
	}
	TBitField res(len);
	for (i = 0; i < ump; i++) {
		res.pMem[i] = bf.pMem[i] & pMem[i];
	}
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField result(BitLen);
	for (int i = 0; i < MemLen; i++) {
		result.pMem[i] = ~pMem[i];
	}
	return result;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	for (int i = 0; i < bf.BitLen; i++) {
		int bit;
		istr >> bit;
		if (bit == 1) {
			bf.SetBit(i);
		}
		else {
			bf.ClrBit(i);
		}
	}
	return istr;
}


ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.BitLen; ++i) {
		if (bf.GetBit(i) == 0) {
			ostr << 0 << " ";
		}
		else cout << 1 << " ";
	}
	return ostr;
}
