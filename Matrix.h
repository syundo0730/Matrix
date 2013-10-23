#ifndef MATRIX_H_
#define MATRIX_H_

#include <iostream>
#include <vector>

template <typename TYPE = double>	//�f�t�H���g�e���v���[�g����
  class Matrix
{
public:
	Matrix(const std::vector<std::vector<TYPE>>& x);	//�񎟌�vector�������Ƃ����R���X�g���N�^
	Matrix(int x = 0, int y = 0);						//�T�C�Y���w�肵��0�Ŗ��߂�R���X�g���N�^
	Matrix(const Matrix& other);	//�R�s�[�R���X�g���N�^

	void operator=(const Matrix& other);		//���
	Matrix<TYPE> operator*(const Matrix& other) const;//�ς�Ԃ�
	Matrix<TYPE> Trans() const;	//�]�u��Ԃ�
	Matrix<TYPE> Inv() const;		//�t�s���Ԃ�

	//�s��̃C���f�b�N�X��0����D

	//���H�̂��߂̊֐��D
	void erase(int r, int c);	//r�s�y��c����폜���ċl�߂�Dr<0�ł���΍s�Cc<0�ł���Η�͍폜���Ȃ��D
	void insert_r(int r, double val);	//r�s�ɑ}������
	void insert_c(int c, double val);	//c��ɑ}������

	Matrix<TYPE> copy_r(int r) const;	//r�s���R�s�[����
	Matrix<TYPE> copy_c(int c) const;	//c����R�s�[����
	
	//�A�N�Z�X�֐�
	TYPE Get(int i, int j) const;	//i�sj��̒l��Ԃ�
	void Set(int i, int j, TYPE value);		//i�sj���value��������
	void Show() const;		//�s���\������

	//�s��̃T�C�Y��Ԃ�
	int Row_Size() const;	//�s�̒�����Ԃ�
	int Colum_Size() const;	//��̒�����Ԃ�

private:
	//�C���f�b�N�X�̃`�F�b�N
	void CheckIndex(int i, int j) const;

private:
	std::vector<std::vector<TYPE>> m_matrix;	//�񎟌�vector�D�s��̖{�́D
	int m_row, m_colum;	//�s��̗v�f��
};

template <typename TYPE>
  Matrix<TYPE>::Matrix(const std::vector<std::vector<TYPE>>& x)		//�R���X�g���N�^
{
	m_matrix = x;
	m_row  = (int)x.size();
	m_colum = (int)x[0].size();
}

template <typename TYPE>
  Matrix<TYPE>::Matrix(int x, int y)						//�R���X�g���N�^
{
	m_matrix.resize(x);
	for(int i = 0; i < (int)m_matrix.size(); i++ ) {
		m_matrix[i].resize(y);
	}

	for(int i = 0; i < (int)m_matrix.size(); i++ ) {
		for(int j = 0; j < (int)m_matrix[i].size(); j++ ) {
			m_matrix[i][j] = 0.0;
		}
	}

	m_row  = x;
	m_colum = y;
}

template <typename TYPE>
  Matrix<TYPE>::Matrix(const Matrix& other)			//�R�s�[�R���X�g���N�^
{
	m_row  = other.m_row;
	m_colum  = other.m_colum;
	m_matrix.clear();
	m_matrix = other.m_matrix;
}

template <typename TYPE>
  void Matrix<TYPE>::operator=(const Matrix& other)	//���
{
	m_row  = other.m_row;
	m_colum  = other.m_colum;
	m_matrix.clear();
	m_matrix = other.m_matrix;
}

template <typename TYPE>
  Matrix<TYPE> Matrix<TYPE>::operator*(const Matrix& other) const	//�ς��v�Z
{
	Matrix<TYPE> seki(m_row, other.m_colum);
	TYPE buf;
	
	if(m_colum != other.m_row){
		std::cerr << "�s�񂪕s���ł��I" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	for(int i = 0; i < m_row; ++i) {
		for(int j = 0; j < other.m_colum; ++j) {
			buf = 0.0;
			for(int k = 0; k < m_colum; ++k) {
				buf += m_matrix[i][k] * other.m_matrix[k][j];
			}
			seki.Set(i, j, buf);
		}
	}
	return seki;
}

template <typename TYPE>
  Matrix<TYPE> Matrix<TYPE>::Trans() const
{
	Matrix<TYPE> trans(m_colum, m_row);	//�s�C��̃T�C�Y�����ւ������̂𐶐�

	for(int i = 0; i < m_row; ++i) {
		for(int j = 0; j < m_colum; ++j) {
			trans.Set(j, i, m_matrix[i][j]);
		}
	}
	return trans;
}

template <typename TYPE>
  Matrix<TYPE> Matrix<TYPE>::Inv() const
{
	vector<vector<TYPE>> a, inv_a(m_row, m_colum);
	TYPE buf;
	int n = m_row;

	if(m_colum != m_row){
		std::cerr << "�s�񂪕s���ł��I" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	a = m_matrix;	//�s����R�s�[

	//�P�ʍs������
	for(int i = 0; i < n; i++){
	  for(int j = 0; j < n; j++){
		inv_a[i][j] = (i == j) ? 1.0 : 0.0;
	  }
	}
	//�|���o���@
	for(int i = 0; i < n; i++){
	  buf = 1.0 / a[i][i];
	  for(int j = 0; j < n; j++){
		a[i][j] *= buf;
		inv_a[i][j] *= buf;
	  }
	  for(int j = 0; j < n; j++){
		if(i != j){
		  buf = a[j][i];
		  for(int k = 0; k < n; k++){
			a[j][k] -= a[i][k] * buf;
			inv_a[j][k] -= inv_a[i][k] * buf;
		  }
		}
	  }
	}
	return Matrix(inv_a);
}

//���H
//�s�C��폜
template <typename TYPE>
  void Matrix<TYPE>::erase(int r, int c)
{
	if(r >= 0) {
		m_matrix.erase(m_matrix.begin() + r, m_matrix.begin() + r + 1);
	}
	if(c >= 0) {
		for(int i = 0, size = m_matrix.size(); i < size; ++i) {
			m_matrix[i].erase(m_matrix[i].begin() + c, m_matrix[i].begin() + c + 1);
		}
	}
	m_row = m_matrix.size();
	m_colum = m_matrix[0].size();
}

//�s�R�s�[
template <typename TYPE>
  Matrix<TYPE> Matrix<TYPE>::copy_r(int r) const
{
	Matrix<TYPE> co(1, m_colum);

	for(int i = 0; i < m_colum; ++i) {
		co.Set(0, i, m_matrix[r][i]);
	}

	return co;
}

//��R�s�[
template <typename TYPE>
  Matrix<TYPE> Matrix<TYPE>::copy_c(int c) const
{
	Matrix<TYPE> co(m_row, 1);

	for(int i = 0; i < m_row; ++i) {
		co.Set(i, 0, m_matrix[i][c]);
	}

	return co;
}

//�s�}��
template <typename TYPE>
  void Matrix<TYPE>::insert_r(int r, double val) {
	vector<double> tmp;
	for(int i = 0, size = m_matrix[r].size(); i < size; ++i) {
		tmp.push_back(val);
	}
	m_matrix.insert(m_matrix.begin() + r, tmp);

	m_row = m_matrix.size();
	m_colum = m_matrix[0].size();
}

//��}��
template <typename TYPE>
  void Matrix<TYPE>::insert_c(int c, double val) {
	for(int i = 0, size = m_matrix.size(); i < size; ++i) {
		m_matrix[i].insert(m_matrix[i].begin() + c, val);
	}

	m_row = m_matrix.size();
	m_colum = m_matrix[0].size();
}

//�A�N�Z�X�֐�(�擾)
template <typename TYPE>
  TYPE Matrix<TYPE>::Get(int i, int j) const
{
	CheckIndex(i, j);
	return m_matrix[i][j];
}

//�A�N�Z�X�֐�(�ݒ�)
template <typename TYPE>
  void Matrix<TYPE>::Set(int i, int j, TYPE value)
{
	CheckIndex(i, j);
	m_matrix[i][j] = value;
}

//�s��̃T�C�Y��Ԃ�
template <typename TYPE>
  int Matrix<TYPE>::Row_Size() const
{
	return m_row;
}

template <typename TYPE>
  int Matrix<TYPE>::Colum_Size() const
{
	return m_colum;
}

//�C���f�b�N�X�̃`�F�b�N
template <typename TYPE>
  void Matrix<TYPE>::CheckIndex(int i, int j) const
{
	if(0 <= i && i < m_row && 0 <= j && j < m_colum){
		//�C���f�b�N�X�͐���
	}else{
		std::cerr << "�C���f�b�N�X���s���ł��I" << std::endl
			<< "�l �F " << i << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

//�s��̕\��
template <typename TYPE>
  void Matrix<TYPE>::Show() const
{
	cout << "-----------------" << endl;
	for(int i = 0; i < m_row; ++i) {
		for(int j = 0; j < m_colum; ++j) {
			cout << m_matrix[i][j] << ' ';
		}
		cout << endl;
	}
	cout << "-----------------" << endl;
}

#endif