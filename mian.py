class Node:
    def __init__(self, key):
        self.key = key
        self.left = None
        self.right = None
        self.height = 1

class AVLTree:
    def getHeight(self, root):
        return root.height if root else 0

    def getBalance(self, root):
        return self.getHeight(root.left) - self.getHeight(root.right) if root else 0

    def rightRotate(self, y):
        x = y.left
        T2 = x.right
        x.right = y
        y.left = T2
        y.height = 1 + max(self.getHeight(y.left), self.getHeight(y.right))
        x.height = 1 + max(self.getHeight(x.left), self.getHeight(x.right))
        return x

    def leftRotate(self, x):
        y = x.right
        T2 = y.left
        y.left = x
        x.right = T2
        x.height = 1 + max(self.getHeight(x.left), self.getHeight(x.right))
        y.height = 1 + max(self.getHeight(y.left), self.getHeight(y.right))
        return y

    def insert(self, root, key):
        if not root:
            return Node(key)
        if key < root.key:
            root.left = self.insert(root.left, key)
        elif key > root.key:
            root.right = self.insert(root.right, key)
        else:  # Duplicate keys not allowed
            return root

        root.height = 1 + max(self.getHeight(root.left), self.getHeight(root.right))
        balance = self.getBalance(root)

        # LL
        if balance > 1 and key < root.left.key:
            return self.rightRotate(root)
        # RR
        if balance < -1 and key > root.right.key:
            return self.leftRotate(root)
        # LR
        if balance > 1 and key > root.left.key:
            root.left = self.leftRotate(root.left)
            return self.rightRotate(root)
        # RL
        if balance < -1 and key < root.right.key:
            root.right = self.rightRotate(root.right)
            return self.leftRotate(root)

        return root

    def getMinValueNode(self, root):
        if root is None or root.left is None:
            return root
        return self.getMinValueNode(root.left)

    def delete(self, root, key):
        if not root:
            return root

        if key < root.key:
            root.left = self.delete(root.left, key)
        elif key > root.key:
            root.right = self.delete(root.right, key)
        else:
            if root.left is None:
                return root.right
            elif root.right is None:
                return root.left
            temp = self.getMinValueNode(root.right)
            root.key = temp.key
            root.right = self.delete(root.right, temp.key)

        if not root:
            return root

        root.height = 1 + max(self.getHeight(root.left), self.getHeight(root.right))
        balance = self.getBalance(root)

        # LL
        if balance > 1 and self.getBalance(root.left) >= 0:
            return self.rightRotate(root)
        # LR
        if balance > 1 and self.getBalance(root.left) < 0:
            root.left = self.leftRotate(root.left)
            return self.rightRotate(root)
        # RR
        if balance < -1 and self.getBalance(root.right) <= 0:
            return self.leftRotate(root)
        # RL
        if balance < -1 and self.getBalance(root.right) > 0:
            root.right = self.rightRotate(root.right)
            return self.leftRotate(root)

        return root

    def preOrder(self, root, res):
        if root:
            res.append(root.key)
            self.preOrder(root.left, res)
            self.preOrder(root.right, res)

    def inOrder(self, root, res):
        if root:
            self.inOrder(root.left, res)
            res.append(root.key)
            self.inOrder(root.right, res)

    def postOrder(self, root, res):
        if root:
            self.postOrder(root.left, res)
            self.postOrder(root.right, res)
            res.append(root.key)

if __name__ == "__main__":
    import sys
    input_line = sys.stdin.readline().strip()
    moves = input_line.split()

    tree = AVLTree()
    root = None

    # Process all moves except last (PRE/IN/POST)
    for m in moves[:-1]:
        if m[0] == 'A':
            val = int(m[1:])
            root = tree.insert(root, val)
        elif m[0] == 'D':
            val = int(m[1:])
            root = tree.delete(root, val)

    # Final traversal
    res = []
    if moves[-1] == 'PRE':
        tree.preOrder(root, res)
    elif moves[-1] == 'IN':
        tree.inOrder(root, res)
    elif moves[-1] == 'POST':
        tree.postOrder(root, res)

    if not res:
        print("EMPTY")
    else:
        print(" ".join(map(str, res)))
