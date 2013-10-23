#ifndef MATRIX_H_
#define MATRIX_H_

#include <iostream>
#include <vector>

template <typename TYPE = double>	//デフォルトテンプレート引数
  class Matrix
{
public:
	Matrix(const std::vector<std::vector<TYPE>>& x);	//二次元vectorを引数としたコンストラクタ
	Matrix(int x = 0, int y = 0);						//サイズを指定して0で埋めるコンストラクタ
	Matrix(const Matrix& other);	//コピーコンストラクタ

	void operator=(const Matrix& other);		//代入
	Matrix<TYPE> operator*(const Matrix& other) const;//積を返す
	Matrix<TYPE> Trans() const;	//転置を返す
	Matrix<TYPE> Inv() const;		//逆行列を返す

	//行列のインデックスは0から．

	//加工のための関数．
	void erase(int r, int c);	//r行及びc列を削除して詰める．r<0であれば行，c<0であれば列は削除しない．
	void insert_r(int r, double val);	//r行に挿入する
	void insert_c(int c, double val);	//c列に挿入する

	Matrix<TYPE> copy_r(int r) const;	//r行をコピーする
	Matrix<TYPE> copy_c(int c) const;	//c列をコピーする
	
	//アクセス関数
	TYPE Get(int i, int j) const;	//i行j列の値を返す
	void Set(int i, int j, TYPE value);		//i行j列にvalueを代入する
	void Show() const;		//行列を表示する

	//行列のサイズを返す
	int Row_Size() const;	//行の長さを返す
	int Colum_Size() const;	//列の長さを返す

private:
	//インデックスのチェック
	void CheckIndex(int i, int j) const;

private:
	std::vector<std::vector<TYPE>> m_matrix;	//二次元vector．行列の本体．
	int m_row, m_colum;	//行列の要素数
};

template <typename TYPE>
  Matrix<TYPE>::Matrix(const std::vector<std::vector<TYPE>>& x)		//コンストラクタ
{
	m_matrix = x;
	m_row  = (int)x.size();
	m_colum = (int)x[0].size();
}

template <typename TYPE>
  Matrix<TYPE>::Matrix(int x, int y)						//コンストラクタ
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
  Matrix<TYPE>::Matrix(const Matrix& other)			//コピーコンストラクタ
{
	m_row  = other.m_row;
	m_colum  = other.m_colum;
	m_matrix.clear();
	m_matrix = other.m_matrix;
}

template <typename TYPE>
  void Matrix<TYPE>::operator=(const Matrix& other)	//代入
{
	m_row  = other.m_row;
	m_colum  = other.m_colum;
	m_matrix.clear();
	m_matrix = other.m_matrix;
}

template <typename TYPE>
  Matrix<TYPE> Matrix<TYPE>::operator*(const Matrix& other) const	//積を計算
{
	Matrix<TYPE> seki(m_row, other.m_colum);
	TYPE buf;
	
	if(m_colum != other.m_row){
		std::cerr << "行列が不正です！" << std::endl;
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
	Matrix<TYPE> trans(m_colum, m_row);	//行，列のサイズを入れ替えたものを生成

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
		std::cerr << "行列が不正です！" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	a = m_matrix;	//行列をコピー

	//単位行列を作る
	for(int i = 0; i < n; i++){
	  for(int j = 0; j < n; j++){
		inv_a[i][j] = (i == j) ? 1.0 : 0.0;
	  }
	}
	//掃き出し法
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

//加工
//行，列削除
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

//行コピー
template <typename TYPE>
  Matrix<TYPE> Matrix<TYPE>::copy_r(int r) const
{
	Matrix<TYPE> co(1, m_colum);

	for(int i = 0; i < m_colum; ++i) {
		co.Set(0, i, m_matrix[r][i]);
	}

	return co;
}

//列コピー
template <typename TYPE>
  Matrix<TYPE> Matrix<TYPE>::copy_c(int c) const
{
	Matrix<TYPE> co(m_row, 1);

	for(int i = 0; i < m_row; ++i) {
		co.Set(i, 0, m_matrix[i][c]);
	}

	return co;
}

//行挿入
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

//列挿入
template <typename TYPE>
  void Matrix<TYPE>::insert_c(int c, double val) {
	for(int i = 0, size = m_matrix.size(); i < size; ++i) {
		m_matrix[i].insert(m_matrix[i].begin() + c, val);
	}

	m_row = m_matrix.size();
	m_colum = m_matrix[0].size();
}

//アクセス関数(取得)
template <typename TYPE>
  TYPE Matrix<TYPE>::Get(int i, int j) const
{
	CheckIndex(i, j);
	return m_matrix[i][j];
}

//アクセス関数(設定)
template <typename TYPE>
  void Matrix<TYPE>::Set(int i, int j, TYPE value)
{
	CheckIndex(i, j);
	m_matrix[i][j] = value;
}

//行列のサイズを返す
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

//インデックスのチェック
template <typename TYPE>
  void Matrix<TYPE>::CheckIndex(int i, int j) const
{
	if(0 <= i && i < m_row && 0 <= j && j < m_colum){
		//インデックスは正常
	}else{
		std::cerr << "インデックスが不正です！" << std::endl
			<< "値 ： " << i << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

//行列の表示
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