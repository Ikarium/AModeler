import os

class Model:
	class New_Namespace:
		class New_Structure:
			None

def Model(iteration):
	y = []
	y2 = []
	m = 1.3968

	k = 0.0991

	w = 54498

	Omega = 1.9681

	Gamma = None
	Mu_A = None
	A = [169924]
	P = [0]
	q = 0.7376

	Taxes = 0.1
	Mu_P = 0.1
	def Calc_Gamma(iteration, m, k):
		y = None
		y = m / (1 + k)

		return (y)

	def Calc_Mu_A(iteration, Omega, Gamma, Calc_Gamma):
		y = None
		y = (Gamma - 1) * Omega

		return (y)

	def Calc_A(iteration, A, Mu_A, w, q, Gamma, Calc_Mu_A):
		A += [A[-1] + q * (Mu_A*A[-1]-Gamma*w)]
		

		return (A)

	def Calc_P(iteration, P, A, Mu_A, Gamma, w, q, Mu_P, Taxes, Calc_A):
		P+= [(1-q)*(1-Taxes)*(Mu_A*A[-1]-Gamma*w)+Mu_P*P[-1]]
		

		return (P)

	for iteration in range(50):
		None
		var_Calc_Gamma_y = Calc_Gamma(iteration, m, k)
		Gamma = var_Calc_Gamma_y
		var_Calc_Mu_A_y = Calc_Mu_A(iteration, Omega, Gamma, var_Calc_Gamma_y)
		Mu_A = var_Calc_Mu_A_y
		var_Calc_A_A = Calc_A(iteration, A, Mu_A, w, q, Gamma, var_Calc_Mu_A_y)
		A = var_Calc_A_A
		var_Calc_P_P = Calc_P(iteration, P, A, Mu_A, Gamma, w, q, Mu_P, Taxes, var_Calc_A_A)
		P = var_Calc_P_P
		y = A
		y2 = P

	return (y, y2)

def Export(iteration, A, P):
	file = open('Export.csv', 'w')
	file.write("Iteration,A,P\n")
	for it in range(len(A)):
	    file.write(str(it) + ","+str(A[it]) + ","+str(P[it]) + ",\n")

	return ()

for iteration in range(1):
	None
	var_Model_y, var_Model_y2 = Model(iteration)
	Export(iteration, var_Model_y, var_Model_y2)
