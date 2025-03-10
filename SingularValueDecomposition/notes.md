This is a small project I will have to learn to do it in c.
How about doing it by tonight.



# QR algorithm for eigen values

---

### **QR Algorithm for Eigenvalues**

**Idea:**  
The QR algorithm iteratively factorizes the matrix \( A \) into a product of an orthogonal matrix \( Q \) and an upper triangular matrix \( R \). By then forming the product \( RQ \) and repeating the process, the sequence of matrices converges to an upper triangular matrix whose diagonal entries are the eigenvalues of \( A \).

---

### **Algorithm Steps**

1. **Input:**
   - A square matrix \( A \) of size \( n \times n \).
   - A tolerance value \( \epsilon \) for convergence.
   - A maximum number of iterations, \( \text{max\_iter} \).

2. **Initialization:**
   - Set \( A^{(0)} = A \).
   - Set iteration counter \( k = 0 \).

3. **Iteration:**
   - **While** \( k < \text{max\_iter} \):
     1. Compute the QR factorization of \( A^{(k)} \):
        \[
        A^{(k)} = Q^{(k)} R^{(k)}
        \]
        where \( Q^{(k)} \) is orthogonal and \( R^{(k)} \) is upper triangular.
     2. Form the next iterate:
        \[
        A^{(k+1)} = R^{(k)} Q^{(k)}
        \]
     3. **Convergence Check:**  
        If the off-diagonal elements of \( A^{(k+1)} \) are below the tolerance \( \epsilon \) (or if the change \( \|A^{(k+1)} - A^{(k)}\| \) is below \( \epsilon \)), then break out of the loop.
     4. Increment \( k \) by 1.

4. **Output:**  
   - After convergence, the eigenvalues of \( A \) are approximated by the diagonal entries of \( A^{(k+1)} \).

---

### **Pseudocode**

```plaintext
function QR_Eigenvalues(A, epsilon, max_iter):
    Input: A square matrix A (n x n)
           Tolerance epsilon
           Maximum iterations max_iter
    Output: Approximated eigenvalues of A
    
    A_current = A
    for k = 0 to max_iter - 1:
        [Q, R] = QR_Factorization(A_current)
        A_next = R * Q
        
        if Converged(A_current, A_next, epsilon):
            break
        
        A_current = A_next

    eigenvalues = [ A_current[i][i] for i = 0 to n-1 ]
    return eigenvalues
```

**Helper Function (Convergence Check):**

```plaintext
function Converged(A_old, A_new, epsilon):
    // Check if the change is below a given tolerance
    return Norm(A_new - A_old) < epsilon
```

---

### **Notes:**

- **QR Factorization:**  
  There are several methods to compute the QR factorization (e.g., Gram-Schmidt, Householder reflections, Givens rotations). In practice, Householder reflections are preferred for numerical stability.

- **Convergence Criteria:**  
  In many implementations, one monitors the size of the sub-diagonal elements. Once these become sufficiently small, the diagonal elements can be taken as the eigenvalues.

- **Shifts:**  
  In practical implementations, **shifts** are often used to accelerate convergence (the *QR algorithm with shifts*). This involves modifying \( A^{(k)} \) by subtracting a shift \( \mu I \), performing the QR factorization, and then adding \( \mu I \) back.

- **Symmetric Matrices:**  
  The QR algorithm converges especially well for symmetric matrices, in which case the eigenvalues are real and the algorithm typically converges rapidly.

---

This is a high-level description of one algorithm to compute eigenvalues. In many numerical software libraries (like LAPACK), more advanced versions of the QR algorithm (with shifts and deflation) are used for better performance and stability.






# How to find QR decomposition algo

Below is one common algorithm—the **Householder Reflection method**—to compute the QR factorization of a matrix. This algorithm is numerically stable and works well for both square and rectangular matrices.

---

## **Householder QR Factorization Algorithm**

**Goal:**  
Given an \( m \times n \) matrix \( A \) (with \( m \ge n \)), find an orthogonal matrix \( Q \) and an upper triangular matrix \( R \) such that:
\[
A = QR.
\]

**Idea:**  
At each step, we use a Householder reflection to zero out the subdiagonal elements of one column of \( A \). After processing all \( n \) columns, the resulting matrix is upper triangular (this becomes \( R \)), and the product of the reflectors gives \( Q \).

---

### **Algorithm Steps**

1. **Initialization:**
   - Set \( R^{(0)} = A \).
   - Initialize \( Q \) as the \( m \times m \) identity matrix, \( Q = I_m \).

2. **For \( k = 1 \) to \( n \):**
   
   1. **Extract the vector to be reduced:**
      - Let \( x \) be the subvector of the \( k \)th column of \( R^{(k-1)} \) from row \( k \) to \( m \):
        \[
        x = R^{(k-1)}(k:m,\, k)
        \]
   
   2. **Compute the Householder vector:**
      - Compute the Euclidean norm:
        \[
        \|x\| = \sqrt{x_1^2 + x_2^2 + \cdots + x_{m-k+1}^2}
        \]
      - Let \( \alpha = -\operatorname{sign}(x_1) \|x\| \)  
        (If \( x_1 = 0 \), one can define \( \operatorname{sign}(x_1)=1 \).)
      - Define vector \( e \) as:
        \[
        e = \begin{pmatrix} 1 \\ 0 \\ \vdots \\ 0 \end{pmatrix} \quad (\text{of length } m-k+1)
        \]
      - Set
        \[
        u = x - \alpha e.
        \]
      - Normalize \( u \) to get the Householder vector \( v \):
        \[
        v = \frac{u}{\|u\|}.
        \]

   3. **Form the Householder matrix \( H_k \):**
      - Define:
        \[
        H_k = I_{m-k+1} - 2 vv^T.
        \]
      - To apply \( H_k \) to the full matrix \( R^{(k-1)} \), extend it to an \( m \times m \) matrix \( \tilde{H}_k \) by embedding \( H_k \) in the lower-right corner:
        \[
        \tilde{H}_k = \begin{pmatrix}
        I_{k-1} & 0 \\
        0       & H_k
        \end{pmatrix}.
        \]

   4. **Update \( R \) and \( Q \):**
      - Update the current \( R \) matrix:
        \[
        R^{(k)} = \tilde{H}_k R^{(k-1)}.
        \]
      - Update \( Q \) (note that the overall \( Q \) is the product of the Householder reflectors):
        \[
        Q = Q \tilde{H}_k^T.
        \]
        (Since \( \tilde{H}_k \) is symmetric, \( \tilde{H}_k^T = \tilde{H}_k \).)

3. **Output:**
   - After \( n \) iterations, let:
     \[
     R = R^{(n)}
     \]
     and \( Q \) is as computed. Then,
     \[
     A = QR.
     \]

---

### **Pseudocode**

```plaintext
function [Q, R] = HouseholderQR(A):
    [m, n] = size(A)
    Q = Identity(m)
    R = A

    for k = 1 to n:
        // 1. Extract vector x from R
        x = R[k:m, k]

        // 2. Compute the norm of x
        norm_x = norm(x)

        // 3. Determine alpha
        if x[1] >= 0 then
            alpha = -norm_x
        else
            alpha = norm_x
        end if

        // 4. Compute the Householder vector
        e = [1, 0, ..., 0]^T  // length = m - k + 1
        u = x - alpha * e
        v = u / norm(u)

        // 5. Form the Householder matrix H_k
        H_k = Identity(m - k + 1) - 2 * v * v^T

        // 6. Extend H_k to full size matrix tilde_H
        tilde_H = Identity(m)
        tilde_H[k:m, k:m] = H_k

        // 7. Update R and Q
        R = tilde_H * R
        Q = Q * tilde_H   // tilde_H is symmetric so its transpose equals itself
    end for

    return Q, R
```

---

### **Key Points:**

- **Householder Vector \( v \):**  
  This vector defines the reflection that zeros out all subdiagonal entries in the current column.

- **Stability:**  
  The Householder method is numerically more stable than classical Gram–Schmidt and is widely used in practice.

- **Storage:**  
  In many implementations, the Householder vectors \( v \) are stored in the lower part of the matrix \( R \) to save space, and \( Q \) is formed only when needed.

- **Output \( Q \):**  
  The matrix \( Q \) is orthogonal, meaning \( Q^T Q = I \).

---

This algorithm, when implemented, will provide you with the matrices \( Q \) and \( R \) such that \( A = QR \). If you have any further questions or need additional details, feel free to ask!


