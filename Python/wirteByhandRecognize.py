
# coding: utf-8

# In[1]:


from sklearn.neural_network import MLPClassifier
from sklearn.datasets import  load_digits
from sklearn.model_selection import  train_test_split
from sklearn.metrics import classification_report,confusion_matrix
import numpy as np
import matplotlib.pyplot as plt


# In[2]:


digits = load_digits()#载入数据
x_data = digits.data
y_data = digits.target


# In[3]:


x_train,x_test,y_train,y_test = train_test_split(x_data,y_data)#分割数据1/4位测试数据，3/4位训练数据


# In[11]:


x_data.shape


# In[4]:


mlp = MLPClassifier(hidden_layer_sizes=(100,50),max_iter=500)
mlp.fit(x_train,y_train)


# In[5]:


#数据中心化
def zeroMean(dataMat):
    meanVal = np.mean(dataMat,axis=0)
    newData = dataMat -meanVal
    return newData,meanVal
def pca(dataMat,top):
    newData,meanVal = zeroMean(dataMat)
    covMat = np.cov(newData,rowvar=0)
    eigVals,eigVects = np.linalg.eig(np.mat(covMat))
    eigValIndice = np.argsort(eigVals)
    n_eigValIndice = eigValIndice[-1:-(top+1):-1]
    n_eigVect = eigVects[:,n_eigValIndice]
    lowDataMat = newData*n_eigVect
    reconMat = (lowDataMat*n_eigVect.T)+meanVal
    return lowDataMat,reconMat


# In[9]:


lowDDataMat,reconMat = pca(x_data,2)#降为2个维度


# In[10]:


# 重构的数据
x = np.array(lowDDataMat)[:,0]
y = np.array(lowDDataMat)[:,1]
plt.scatter(x,y,c='r')
plt.show()


# In[12]:


predictions = mlp.predict(x_data)


# In[15]:


# 重构的数据
x = np.array(lowDDataMat)[:,0]
y = np.array(lowDDataMat)[:,1]
plt.scatter(x,y,c=y_data)
plt.show()


# In[16]:


lowDDataMat,reconMat = pca(x_data,3)#降成3个维度


# In[17]:


from mpl_toolkits.mplot3d import  Axes3D


# In[19]:


x = np.array(lowDDataMat)[:,0]
y = np.array(lowDDataMat)[:,1]
z = np.array(lowDDataMat)[:,2]
ax = plt.figure().add_subplot(111,projection = '3d')
ax.scatter(x,y,z,c=predictions,s = 10)#点为红色三角形
plt.show()

